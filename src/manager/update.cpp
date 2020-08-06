#include "manager.hpp"

void manager::update_all()
{
	while (1)
	{
		mutex.lock();
		update_jobs();
		container_save();
		update_server();
		update_group();
		mutex.unlock();

		sleep(1);
	}
}

void manager::container_save()
{
	static int sec;
	sec++;

	// guarda informacion a json cada 10 seg
	if (sec > 10)
	{
		jwrite(path + "/etc/info.json", struct_to_json());
		sec = 0;
	}
}

QString manager::update_server_thread(QJsonArray recv)
{
	if (not recv.empty())
	{
		QString name = recv[0].toString();
		QString host = recv[1].toString();
		int cpu = recv[2].toInt();
		int ram = recv[3].toInt();
		int temp = recv[4].toInt();
		QString system = recv[5].toString();
		QString mac = recv[6].toString();
		float ram_total = recv[7].toDouble();
		float ram_used = recv[8].toDouble();
		int cpu_cores = recv[9].toInt();
		QString log = recv[10].toString();
		QString sshUser = recv[11].toString();
		QString sshPass = recv[12].toString();

		int response_time = time(0);

		if (not is_struct(servers, name))
		{
			QList<inst_struct *> instances;

			for (int i = 0; i < 16; ++i)
			{
				inst_struct *ins = new inst_struct;
				ins->index = i;
				ins->status = 0;
				ins->reset = 0;
				ins->job_task = "...";
				instances.push_back(ins);
			}

			server_struct *server = new server_struct;

			server->name = name;
			server->status = "idle";
			server->host = host;
			server->system = system;
			server->cpu = cpu;
			server->cpu_cores = cpu_cores;
			server->ram = ram;
			server->ram_used = ram_used;
			server->ram_total = ram_total;
			server->temp = temp;
			server->mac = mac;
			server->response_time = response_time;
			server->instances = instances;
			server->max_instances = 1;
			server->sshUser = sshUser;
			server->sshPass = sshPass;
			server->schedule = "0-0";
			server->schedule_state_0 = 1;
			server->schedule_state_1 = 1;
			server->log = log;
			servers.push_back(server);
		}

		else
		{
			auto server = find_server(name);

			server->ram_used = ram_used;
			server->cpu = cpu;
			server->ram = ram;
			server->temp = temp;
			server->host = host;
			server->log = log;
			server->response_time = response_time;
		}
	}

	// se usa mutex para que cuando se este generando el json no se este
	// copiando informacion en "preferences" si no el programa se cae
	QString ret;

	ret = jots(preferences);

	return ret;
	//---------------------------------------------------------------
}

void manager::update_server()
{
	int t1 = time(0);

	QJsonArray serverList;

	for (auto server : servers)
	{
		serverList.push_back(server->name);

		// set status server
		int t2 = server->response_time;
		int response_time = t1 - t2;

		// si el porcentaje de la ram es mayor a 90 %, el tiempo de espera es mas para que no quede aucente le servidor
		int response;
		if (server->ram > 90)
		{
			response = 120;
		}
		else
		{
			response = 30;
		}
		//--------------------------------------------------------------------------------------------------

		if (response_time > response)
		{ // si no responde en 10 segundo queda ausente
			server->status = "absent";
		}
		else
		{
			server->status = "idle";
		}

		for (int i = 0; i < server->max_instances; ++i)
		{
			if (server->instances[i]->status == 3)
				threading([=]() {
					sleep(7);
					if (server->instances[i]->status == 3)
					{
						server->instances[i]->status = 0;
						server->instances[i]->job_task = "...";
					}
				});
		}
		//--------------------------------

		//schedule server
		if (not iTime(server->schedule))
		{
			if (server->schedule_state_0)
			{
				server_set_state(server, false);
				server->schedule_state_0 = false;
				server->schedule_state_1 = true;
			}
		}

		else
		{
			if (server->schedule_state_1)
			{

				server_set_state(server, true);
				server->schedule_state_1 = false;
				server->schedule_state_0 = true;
			}
		}

		//-----------------------------
	}

	preferences["servers"] = serverList;
}

bool manager::iTime(QString schedule)
{
	int start = 0, end = 0;

	auto range = schedule.split("-");
	start = range[0].toInt();
	end = range[1].toInt() - 1;

	// si no esta en el rango de tiempo inTime es 0
	int hora = currentDateTime(6).toInt();
	if (start > end)
	{
		if ((hora >= start) or (hora <= end))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if ((start <= hora) <= end)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//----------------------------------------------
}

void manager::update_group()
{
	// Obtiene todos los grupos que estan activos
	QStringList groups_used;
	for (auto job : jobs)
	{
		if (job->active_task)
		{
			for (QString sg : job->server_group)
			{
				groups_used.push_back(sg);
			}
		}
	}
	//------------------------------------------------
	QJsonArray grouplist;

	for (auto group : groups)
	{
		grouplist.push_back(group->name);
		int totaMachine = 0, activeMachine = 0;

		for (auto _server : group->server)
		{

			if (is_struct(servers, _server->name))
			{
				auto server = find_server(_server->name);

				totaMachine++;
				if (server->status == "absent")
				{
					_server->status = false;
				}
				else
				{
					_server->status = true;
					activeMachine++;
				}
			}
			else
			{
				erase_by_name(group->server, _server->name);
			}
		}
		bool server_status;
		if (groups_used.contains(group->name))
			server_status = true;
		else
			server_status = false;

		group->status = server_status;
		group->totaMachine = totaMachine;
		group->activeMachine = activeMachine;
	}

	preferences["groups"] = grouplist;
}

void manager::update_jobs()
{
	for (auto job : jobs)
	{
		int waiting = job->tasks - (job->suspended_task + job->progres + job->active_task);
		job->waiting_task = waiting;

		if (job->failed_task)
		{
			threading([=]() {
				job->status = "Failed";
				sleep(7);
				job->status = "Queue";
			});

			job->failed_task = 0;
		}
		else
		{
			if ((job->status == "Suspended") or (job->status == "Completed") or (job->status == "Failed") or (job->status == "Concatenate"))
				;
			else if (job->active_task)
				job->status = "Rendering...";
			else
				job->status = "Queue";
		}

		if (job->active_task)
		{
			// tiempo actual de inicio de los dias
			int current_time = QTime::currentTime().msecsSinceStartOfDay();
			// ---------------------------

			int add = 0;
			if (job->time_elapsed_running)
				// tiempo actual menos el ultimo tiempo registrado
				add = current_time - job->last_time;

			job->time_elapsed += add;

			// evita que el tiempo se dispare cuando inicia la cuenta,
			// esto pasa cuando se resta el 'current_time' por 'job->last_time' que
			// al inicio es cero.
			if (job->time_elapsed > 50000000)
				job->time_elapsed = 0;
			// ---------------------------

			int estimated_time;
			if (job->progres)
			{
				// calcula el tiempo estimado
				int remaining = job->tasks - job->progres;
				estimated_time = (job->time_elapsed * remaining) / job->progres;
				if (estimated_time > job->estimated_time_ms)
					if (job->estimated_time_ms)
						estimated_time = job->estimated_time_ms;
			}
			else
				estimated_time = 0;

			job->estimated_time_ms = estimated_time;

			// convierte los milisegundo en segundos y luego al formato de tiempo
			// en string, para el tiempo estimado y restante.
			if (estimated_time)
				job->estimated_time = secToTime(estimated_time / 1000);
			else
				job->estimated_time = "...";
			job->total_render_time = secToTime(job->time_elapsed / 1000);
			// ------------------------------

			job->last_time = current_time;
			job->time_elapsed_running = true;
		}
		else
		{
			job->time_elapsed_running = false;
			job->estimated_time = "...";
		}
	}
}