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

QJsonObject manager::recieve_monitor_thread( QJsonObject recv ){

	auto pks = recv[0].toString(), id = recv[1].toString();

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
	json active_server;

	for ( auto task : job->task ){
		if ( not ( task->server == "...") ){
			if ( task->status == "active" ){
				auto s = split( task->server, ":" );	
				active_server[s[0]].push_back( stoi(s[1]) );				
			}
		}
	}

	if ( _delete ){	
		erase_by_name( jobs, job->name );
	}

	for ( auto server : servers ){
		json ins = active_server[ server->name ];

		if ( not ins.empty() ){
			tcpClient( server->host, 7001, ins, 3 );			
		}
	}
}

void manager::jobAction( QJsonObject pks ){

	for ( auto _job : pks.toArray() ){

        QString job_name = _job[0].toString(); 
        QString job_action = _job[1].toString();

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

QJsonObject manager::jobOptions( QJsonObject pks ){
	int num = 0;
	string _name;
	for ( auto _job : pks.toArray() ){

		QString job_name = _job[0].toString();
		QJsonArray options = _job[1].toArray(); 
		QString action = _job[2].toString();

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
			int _first_frame = options[5];
			int _last_frame = options[6];
			int _task_size = options[7];

			// el nombre se repite se pone un numero al final del nombre 
			string name = options[8];
			if ( not num  ) job->name = name;
			else job->name = name + "_" + to_string(num);
			num++;
			//------------------------------------------------

			// si el first_frame, last_frame y task_size no se modifican no crea las tares otra vez
			if ( ( job->first_frame != _first_frame ) or ( job->last_frame != _last_frame )   
				or ( job->task_size != _task_size ) ){

				job->first_frame = _first_frame;
				job->last_frame = _last_frame;
				job->task_size = _task_size;

				// obtiene los frames segun el estado
				vector <int> finished;
				vector <int> suspended;
				for ( auto task : job->task ){
					if ( ( task->status == "suspended" ) )
						for (int i = task->first_frame; i <= task->last_frame; ++i)
							suspended.push_back(i);

					if ( ( task->status == "finished" ) )
						for (int i = task->first_frame; i <= task->last_frame; ++i)
							finished.push_back(i);
				}
				//----------------------------------------------------

				auto tasks = make_task( job->first_frame, job->last_frame, job->task_size );
				job->task = tasks;
				job->waiting_task = tasks.size();
				job->tasks = tasks.size();

				int progres = 0;
				for ( auto task : job->task ){ 
					int task_size = task->last_frame - task->first_frame + 1;

					int _finished = 0;
					int _suspended = 0;

					for (int i = task->first_frame; i <= task->last_frame; ++i){
						for ( int f : finished ) if ( i == f ) _finished++;
						for ( int s : suspended ) if ( i == s ) _suspended++;
					}

					if ( task_size == _finished ){
						task->status = "finished";
						progres++;
					}
					if ( task_size == _suspended )
						task->status = "suspended";

				}
				job->progres = progres;
			}//----------------------------------------

			resetAllServer();
		}

		if ( action == "read" ){ 
			QJsonObject _server;
			for ( string s : job->server ) _server.push_back(s);
			//----------------------------------------------------
			QJsonObject _server_group;
			for ( string sg : job->server_group ) _server_group.push_back(sg);
			//----------------------------------------------------
			return { _server, _server_group, job->priority, job->comment, job->instances, 
					job->task_size, job->name, job->first_frame, job->last_frame };
		}

	}

	return {};
}

QJsonObject manager::serverAction( QJsonObject pks ){

	for ( auto _server : pks.toArray() ){

        QString name = _server[0].toString();
        QString server_action = _server[1].toString();
        int info =  _server[2].toInt();
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
		QJsonObject kill_ins;

        for (int i = 0; i < server->max_instances; ++i){
            auto ins = server->instances[i];
			ins->status = 2;
			ins->job_task = "...";
			kill_ins.push_back(i);
        }

        tcpClient( server->host, 7001, kill_ins, 3 );

    }
}

QJsonObject manager::serverOptions( QJsonObject pks ){

	for ( auto _server : pks.toArray() ){

        QString name = _server[0].toString();
        QJsonArray recv = _server[1].toArray();
        QString action =  _server[2].toString();

		auto server = find_struct( servers, name );

		if ( action == "read" ){
			return { server->schedule };
        }

		if ( action == "write" ){
			server->schedule = recv[0].toString();
        }
    }

	return {};
}

void manager::groupAction( QJsonObject pks ){

	QJsonObject group_list = pks[0].toObject(); 
	QJsonObject group_machine = pks[1].toObject();
	QString group_action = pks[2].toString();

	if ( group_action == "addMachine" ){

		for ( QJsonArray _group : group_machine ){
			QString name = _group[0].toString(); 
			QJsonArray serverList = _group[1].toArray();

			auto group = find_struct( groups, name );

			for ( auto server : serverList ){
				if ( not is_struct( group->server, server.toString() ) ){

				string status = find_struct( servers, server.toString() )->status;
				bool _status = true;

				if ( status == "absent" ){ _status = false; }

					serverFromGroupStruct *sg = new serverFromGroupStruct;
					sg->name = server.toString();
					sg->status = _status;

					group->server.push_back( sg );

				}
			}

		}

	}

	if ( group_action == "delete" ){
		// elimina servers del grupo

		for ( auto _group : group_machine ){
            QString name = _group[0].toString();
            QString server = _group[1].toString();

            auto group = find_struct( groups, name);

            erase_by_name( group->server, server );

		}

		for ( auto group : group_list ){
			erase_by_name( groups, group.toString() );
		}
		//------------------------------------------
    }
}

void manager::taskAction( QJsonObject pks ){

	for ( auto _task : pks.toArray() ){
        QString job_name = _task[0].toString();
        QString task_name = _task[1].toString();
        QString task_action = _task[2].toString();
        
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

void manager::groupCreate( QJsonObject pks ){

	QString group_name_in = pks[0].toString(); 
	int totaMachine = pks[1].toInt(); 
	int activeMachine = pks[2].toInt(); 
	QJsonArray server = pks[3].toArray();

	QString group_name = group_name_in;

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
		sg->name = name.toString() ;
		sg->status = 0;
		_server.push_back( sg );
	}
    group->server = _server;
	//-----------------------------------------------

    groups.push_back( group );
}

QJsonObject manager::preferencesAction( QJsonObject pks ){

	if ( pks.toString() == "read" ){
		return  jread( "../../etc/preferences.json" );

	}
	else{
		preferences[ "paths" ] = pks.toString();
		jwrite( "../../etc/preferences.json", preferences );
	}

	return {};
}

QJsonObject manager::jobLogAction( QJsonObject pks ){

	QString server_name = pks.toString();
	auto server = find_struct( servers, server_name );
	QString result = server->log;

	return result;
}

