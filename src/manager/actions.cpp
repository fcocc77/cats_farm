#include "manager.hpp"

// Recive la informacion del monitor
void manager::reset_all_servers()
{
	// Resetea todos los servidores para que empiece otra vez
	for (auto server : servers)
		for (int i = 0; i < server->max_instances; i++)
			server->instances[i]->reset = 1;

	reset_render = true;
	//--------------------------------------------------
}

QString manager::recieve_monitor_thread(QJsonArray recv)
{

	QString id = recv[0].toString();
	QJsonArray pks = recv[1].toArray();

	if (not recv.empty())
	{
		if (id == "jobAction")
			job_action(pks);
		if (id == "jobOptions")
			return job_options(pks);
		if (id == "serverOptions")
			return server_options(pks);
		if (id == "serverAction")
			return server_action(pks);
		if (id == "groupAction")
			group_action(pks);
		if (id == "taskAction")
			task_action(pks);
		if (id == "groupCreate")
			group_create(pks);
		if (id == "preferences")
			return preferences_action(pks);
		if (id == "jobLogAction")
			return job_log_action(recv[0].toString());
	}

	return "";
}

QString manager::preferences_action(QJsonArray _pks)
{
	QString action = _pks[0].toString();

	if (action == "read")
		return jots(jread(path + "/etc/preferences.json"));
	else
	{
		QJsonObject pks = _pks[1].toObject();
		preferences["paths"] = pks;
		jwrite(path + "/etc/preferences.json", preferences);
	}

	return "";
}

