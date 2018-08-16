#include "manager.h"

void manager::render_job(){
	while (1){
		reset_render = false;

		// lee las listas del trabajo creadas y las ordena segun prioridad
		sort( jobs.begin(), jobs.end(), [] ( job_struct *a, job_struct *b){
			bool cmp;
			cmp = a->priority < b->priority;
			// si prioridad es igual compara el submit_start
			if ( a->priority == b->priority ){
				cmp = a->submit_start < b->submit_start;
			}
			return cmp;} 
	    );
		//----------------------------------------------------------------

		//si el trabajo esta en cola se manda a render
		for ( auto job : jobs ){
			debug("manager::render_job: for job");
			// hace una lista con los servidores listos para renderear
			vector <string> machinesList;
			for ( auto sg : job->server_group ){
				if ( not groups.empty() ){ 
					auto group = find_struct( groups, sg );

					for ( auto s : group->server )
						machinesList.push_back( s->name );
				}				
			}

			for ( auto s : job->server )
				if ( not in_vector( s, machinesList ) ) 
					machinesList.push_back(s);
			//------------------------------------------------------

			for ( auto server : servers ){
				debug("manager::render_job: for server");
				bool serverOK = 0;
				if ( in_vector( server->name, machinesList ) ){
					serverOK = 1;
				} //----------------------------------

				for ( int ins=0; ins < server->max_instances; ins++ ){

					// si la instancia esta dentro del trabajo renderea
					bool instanceOK = 0;
					for ( int i=0; i < job->instances; i++ ){
						if ( i == ins ){
							instanceOK = 1;
						}
					}			
					//-------------------------------------------------

					if ( serverOK ){
						if ( instanceOK ){
							if ( job->status == "Queue" or job->status == "Rendering..." or job->status == "Failed" ){

								// Veta  los servers que fallaron mas de dos veces
								int vetoed_times = 0;
								for ( auto s : job->vetoed_servers ){
									if ( server->name == s ){
										vetoed_times++;
									}
								}
								//------------------------------------------

								if ( not ( vetoed_times >= 3 ) ){// este numero es la cantidad de veces que puede fallar el servidor antes que de bloquee

									if ( job->waiting_task ){
										auto instance = server->instances[ins];
										int active = instance->status;
										if ( not active ){
											instance->status = 1;
											threading( &manager::render_task, this, server, instance, job ); // this es que es un hilo dentro de una classe
											usleep(200000); //0.2 sec

										}
									}
								}
							}
						}
					}

					if ( reset_render ) break;
				}

				if ( reset_render ) break;
			}

			if ( reset_render ) break;

		}

		sleep(1);
	}
}

void manager::render_task( server_struct *server, inst_struct *instance, job_struct *job ){
	debug("manager::render_task.");
	instance->reset = 0;

	auto software = job->software;
	auto extra = job->extra;
	auto render = job->render;

	auto system = server->system;
	auto jobSystem = job->system;

	auto project = job->project;

	bool Completed = false;
	string result = "";

	// renderea las tareas por cada trabajo, solo si status es igual a waiting
	for ( auto task : job->task ){

		auto first_frame = task->first_frame;
		auto last_frame = task->last_frame;

		auto status = task->status;

		// pone en la instancia que job se esta rendereando	
		instance->job_task = job->name + " : " + task->name;
		//-------------------------------------------------

		if ( instance->reset ){ break; }
		//----------------------------------------
		if ( server->status == "absent" ){ break; }
		//-----------------------------------------

		// envia la tarea al servidor para que la procese, si es que el status es waiting
		if ( status == "waiting" ){
			//pone en activo la tarea
			task->status = "active";
			job->active_task++;
			task->server = server->name + ":" + to_string( instance->index );
			int time1 = time(0);
			//------------------------------
			job->status = "Rendering...";

			string total_frame = to_string( last_frame - first_frame + 1 );

			// Envia a renderar la tarea al servidor que le corresponde
			json pks = { project, software, instance->index, first_frame, last_frame, jobSystem, extra, render };

			json result =  tcpClient( server->host, 7001, pks, 0 );

			if ( not ( result == "ok" ) ){
				if ( result == "kill" ){

					int active_task = job->active_task-1; 
					if ( active_task >=0 ){
						job->active_task = active_task;
					}

					task->status = "waiting";
					task->time = "...";
					task->server ="...";

					break;
				}

				else { // if failed
					result == "failed";

					job->vetoed_servers.push_back( server->name );

					job->failed_task = 1;

					int active_task = job->active_task-1;       
					if ( active_task >= 0 ){
						job->active_task = active_task;
					}

					task->status = "failed";
					sleep(4);
					task->status = "waiting";
					task->time = "...";
					task->server = "...";

					break;
				}
			}
			//-------------------------------------------------------------

			int active_task = job->active_task-1; 
			if ( active_task >=0 ){
				job->active_task = active_task;
			}

			task->status = "finished";
			//----------------------------
			int seconds = time(0) - time1;
			task->time = timeStruct(seconds);
			//-----------------------------------

			auto tasks = job->tasks;
			int before_progres = job->progres;
			int after_progres;
			//cuando el valor del progreso ya es igual al total de las tareas, no le suma 1 al progreso, si no el progreso queda arriba de 100%
			if ( not ( tasks == before_progres ) ){
				after_progres = before_progres+1;
				job->progres = after_progres;
			}
			else{
				after_progres = before_progres;
			}
			//----------------------------------------------------

			if ( tasks == after_progres){
				Completed=true;
			}
		}
	}

	reset_render = true;

	if ( result == "failed" ){
		instance->status = 3;
		job->status = "Failed";
	}

	else{
		//cuando uno pone el server inactivo se mantiene inactivo
		if ( not ( instance->status == 2 ) ){
			instance->status = 0;
			instance->job_task = "...";
		}
	}

	if ( Completed ){
		debug("manager::render_task: Completed");
		if ( software == "Nuke" or software == "Fusion" ){

			string ext = extra.substr( extra.length() - 3 );

			if ( ext == "mov" ){
				job->status = "Concatenate";

				// obtiene nombre de carpeta de renders
				string _dirname = os::dirname(extra);
				string _basename = os::basename(extra);
				_basename = replace( _basename, ".mov", "" );
				//-----------------------------------------------------

				json system_path = preferences["paths"]["system"];

				//obtiene ruta correcta
				string src_path, dst_path, extra;

				for ( string p1 : system_path ){
					for ( string p2 : system_path){
						extra = replace( project, p1, p2 );

						if ( os::isfile( extra ) ){
							src_path = p1;
							dst_path = p2;
							break;
						}

					}

					if ( os::isfile( extra ) ){
						break;
					}
				}
				//--------------------------------------

				_dirname = replace( _dirname, src_path, dst_path );
				if ( os::isdir( _dirname ) ){
					concat( _dirname + "/" + _basename );
				}

			}
		}
		//------------------------------------------------------------

		string submit_finish = "tiempo";// time.strftime("%Y-%m-%d   %H:%M:%S")

		job->submit_finish = submit_finish; 
		job->status = "Completed";
	}
}

