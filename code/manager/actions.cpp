#include "manager.h"

// Recive la informacion del monitor
void manager::resetAllServer(){
	// Resetea todos los servidores para que empiece otra vez
	for ( auto server : servers ){ 
		for ( int i=0; i<server->max_instances; i++){
			server->instances[i]->reset = 1;
		}
	}
	reset_render=true;
	//--------------------------------------------------
}

json manager::recieve_monitor_thread( json recv ){

	auto pks = recv[0], id = recv[1];

	if ( not pks.empty() ){

		if ( id =="jobAction" ){ jobAction( pks ); }
		if ( id == "jobOptions" ){ return jobOptions( pks ); }
		if ( id == "serverOptions" ){ return serverOptions( pks ); }
		if ( id == "serverAction" ){ return serverAction( pks ); }
		if ( id == "groupAction" ){ groupAction( pks ); }
		if ( id == "taskAction" ){ taskAction( pks ); }
		if ( id == "groupCreate" ){ groupCreate( pks ); }
		if ( id == "preferences" ){ return preferencesAction( pks ); }
		if ( id == "jobLogAction" ){ return jobLogAction( pks ); }

	}

	return {};
}

void manager::kill_tasks( job_struct *job, bool _delete ){
	
	//desactica servers que estaban activos por este job
	vector <string> active_server;

	for ( auto task : job->task ){
		if ( not ( task->server == "...") ){
			if ( task->status == "active" ){
				active_server.push_back( task->server );
			}
		}
	}
	
	//active_server.push_back("z:");

	vector <string>  a; 
	string c, server_name, server_instance;
	struct name_instance { string name; vector <string> instances; };
	vector <name_instance> active_server_instance;

	sorted( active_server );
	
	for ( auto server : active_server  ){
		
		auto s = split( server, ":");
		server_name = s[0]; server_instance = s[1];
		if ( a.empty() ){ c = server_name; }

		if ( not ( server_name == c ) ){
			name_instance ni = { c, a };
			active_server_instance.push_back( ni );
		}
		a.push_back( server_instance );
		c = server_name;

	}
	if ( _delete ){	
		erase_by_name( jobs, job->name );
	}

	for ( auto s : active_server_instance ){
		//auto server =  find_struct( servers, s.name );
		//tcpClient( server->host, 7001, s.instances, 3 );
	}
}

void manager::jobAction( json pks ){

	for ( auto _job : pks ){

        string job_name = _job[0]; 
        string job_action = _job[1];

		auto job = find_struct( jobs, job_name );

		if ( job_action == "delete" ){
			kill_tasks( job, true );
		}

		if ( job_action == "suspend" ){	
			if ( not ( job->status == "Completed" ) ){
				job->status = "Suspended";
			}
			kill_tasks( job, false );
        }

		if ( job_action == "unlock" ){
			job->vetoed_servers = {};
		}

		if ( job_action == "resume" ){
			if ( not ( job->status == "Completed" ) ){
				job->status = "Queue";
			}
        }

		if ( job_action == "restart" ){					

			kill_tasks( job, false );

			job->status = "Queue";
			job->progres = 0;
			job->suspended_task = 0;
			job->active_task = 0;
			job->timer = "...";
			job->timer2 = "...";
			job->submit_finish = "...";
			job->total_render_time = "...";
			job->vetoed_servers = {};

			// reset tasks
			for ( auto task : job->task ){
				task->status = "waiting";
				task->server = "...";
				task->time = "...";
			}
			//-------------------------------------
			
        }

		resetAllServer();

    }
}

json manager::jobOptions( json pks ){

	for ( auto _job : pks ){ 
		json job_name = _job[0];
		json options = _job[1]; 
		json action = _job[2];


		auto job = find_struct( jobs, job_name );

		if ( action == "write" ){
			job->server = {};
			for( auto s :  options[0] ) job->server.push_back(s);
			//------------------------
			job->server_group = {};
			for( auto sg :  options[1] ) job->server_group.push_back(sg);
			//------------------------			
			job->priority = options[2];
			job->comment = options[3];
			job->instances = options[4];
			job->first_frame = options[5];
			job->last_frame = options[6];
			job->task_size = options[7];
			job->name = options[8];


			auto tasks = make_task( job->first_frame, job->last_frame, job->task_size );
			job->task = tasks;
			job->waiting_task = tasks.size();
			job->tasks = tasks.size();
			
			resetAllServer();
		}


		if ( action == "read" ){ 
			json _server;
			for ( string s : job->server ) _server.push_back(s);
			//----------------------------------------------------
			json _server_group;
			for ( string sg : job->server_group ) _server_group.push_back(sg);
			//----------------------------------------------------
			return { _server, _server_group, job->priority, job->comment, job->instances, 
					job->task_size, job->name, job->first_frame, job->last_frame };
		}

	}
	
	return {};
}

json manager::serverAction( json pks ){

	for ( auto _server : pks ){

        auto name = _server[0], server_action = _server[1], info =  _server[2];
		auto server = find_struct( servers, name );

		if ( server_action == "max_instances" ){
			server->max_instances = info;
		}

		if ( server_action == "inactive" ){
			serverSetState( server, false );
		}

		if ( server_action == "reactive" ){
			serverSetState( server, true );
		}

		if ( server_action == "delete" ){
			if ( not ( server->status == "active" ) ){
				erase_by_name( servers, name );
			}
        }

		if ( server_action == "ssh" ){
			return { server->sshUser, server->sshPass, server->host };
        }
    }

	return {};
}

void manager::serverSetState( server_struct *server, bool state ){

	if ( state ){
        for (int i = 0; i < server->max_instances; ++i){
			auto ins = server->instances[i];
			if ( ins->status == 2 ){
				ins->status = 0;
				ins->job_task = "...";
			}
        }
    }

	else{
		json kill_ins;

        for (int i = 0; i < server->max_instances; ++i){
            auto ins = server->instances[i];
			ins->status = 2;
			ins->job_task = "...";
			kill_ins.push_back(i);
        }

        tcpClient( server->host, 7001, kill_ins, 3 );

    }
}

json manager::serverOptions( json pks ){

	for ( auto _server : pks ){

        auto name = _server[0], recv = _server[1], action =  _server[2];

		auto server = find_struct( servers, name );

		if ( action == "read" ){
			return { server->schedule };
        }

		if ( action == "write" ){
			server->schedule = recv[0];
        }
    }

	return {};
}

void manager::groupAction( json pks ){

	json group_list = pks[0], group_machine = pks[1], group_action = pks[2];

	if ( group_action == "addMachine" ){

		for ( json _group : group_machine ){
			string name = _group[0]; 
			json serverList = _group[1];

			auto group = find_struct( groups, name );

			for ( auto server : serverList ){
				if ( not is_struct( group->server, server ) ){

				string status = find_struct( servers, server )->status;
				bool _status = true;

				if ( status == "absent" ){ _status = false; }

					serverFromGroupStruct *sg = new serverFromGroupStruct;
					sg->name = server;
					sg->status = _status;

					group->server.push_back( sg );

				}
			}

		}

	}

	if ( group_action == "delete" ){
		// elimina servers del grupo
		 
		for ( auto _group : group_machine ){
            auto name = _group[0], 
            server = _group[1];

            auto group = find_struct( groups, name);

            erase_by_name( group->server, server );

		}

		for ( auto group : group_list ){
			erase_by_name( groups, group );
		}
		//------------------------------------------
    }
}

void manager::taskAction( json pks ){

	for ( auto _task : pks ){
        string job_name = _task[0], task_name = _task[1], task_action = _task[2];
        auto job = find_struct( jobs, job_name );
        auto task = find_struct( job->task, task_name );

		if ( task_action == "suspend" ){
			if ( task->status == "waiting" ){
				job->suspended_task += 1;
				task->status = "suspended";
				job->status = "Queue ";
            }
		}

		if ( task_action == "resume" ){
			if ( not ( task->status == "active" ) ){

				if ( task->status == "suspended" ){
					job->suspended_task -= 1;
				}

				if ( task->status == "finished" ){
					job->progres -= 1;
				}

				if ( task->status == "failed" ){
					job->failed_task -= 1;
				}

				task->status = "waiting";
				task->server = "...";
				task->time = "...";
				job->status = "Queue";
            }
        }
    }
}

void manager::groupCreate( json pks ){

	string group_name_in = pks[0]; 
	int totaMachine = pks[1]; 
	int activeMachine = pks[2]; 
	json server = pks[3];

	string group_name = group_name_in;

	int pad = 0;
	while (1){
		pad += 1;
		if ( is_struct( groups, group_name ) ){
			group_name = group_name_in + "_" + to_string(pad);
		}
        else { break; }
    }

    group_struct *group = new group_struct;

    group->name = group_name;
    group->status = 0; 
    group->totaMachine = totaMachine;
    group->activeMachine = activeMachine;

	//crea vector de server group
	vector <serverFromGroupStruct*> _server;
    for ( auto name : server ){
		serverFromGroupStruct *sg = new serverFromGroupStruct;
		sg->name = name ;
		sg->status = 0;
		_server.push_back( sg );
	}
    group->server = _server;
	//-----------------------------------------------

    groups.push_back( group );

    qDebug() << "create in manager";
}

json manager::preferencesAction( json pks ){

	if ( pks == "read" ){
		return  jread( "../../etc/preferences.json" );
		
	}
	else{
		preferences[ "paths" ] = pks;
		jwrite( "../../etc/preferences.json", preferences );
	}

	return {};
}

json manager::jobLogAction( json pks ){

	string server_name = pks;
	auto server = find_struct( servers, server_name );
	string result = server->log;




	return result;
}

