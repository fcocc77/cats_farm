#include "manager.h"

void manager::init(){
    // Carga la informacion guardada de Jobs, Servers y Groups

    json info = jread( "../../etc/info.json" );
    if ( not info.empty() ){
        json_to_struct( info );
    }

    //-----------------------------------------------
    reactive_all();
    // Recive la informacion del suministrador y crea un jobs con sus tareas

	tcpServer( 7000, &manager::server_tcp, this );

	threading( &manager::update_all, this );

	threading( &manager::render_job, this );
}

json manager::server_tcp( json pks, int input ){

	if ( input == 0 ) return make_job( pks );
	if ( input == 1 ) return update_server_thread( pks );
	if ( input == 2 ) return sendToMonitor_thread( pks );
	if ( input == 3 ) return recieve_monitor_thread( pks );

	return {};
}

void manager::reactive_all(){

	// si el manager se apaga cuando esta rendereando se resetea todo a cola
	for ( auto& job : jobs){
		if ( job->status == "Rendering..." ){
			job->status = "Queue";
		}

		for ( auto& task : job->task ){
			if ( task->status == "active" ){
				task->status = "waiting";
				job->active_task -= 1;				
			}

		}
	}

	for ( auto& server : servers ){
		for ( auto& instance : server->instances ){
			if ( instance->status == 1 ){
				 instance->status = 0;
			} 
			instance->reset = 0;
		}

	}

	//-------------------------------------------------------------
}

json manager::make_job( json recv ){
	debug("manager::make_job.");

	string _job_name = recv[0]; 
	//------------------------------
	vector <string> _server;
	if ( recv[1] != "None" ) _server.push_back( recv[1] );
	//------------------------------
	vector <string> _server_group;
	if ( recv[2] != "None" ) _server_group.push_back( recv[2] );
	//------------------------------
	int _first_frame = recv[3]; 
	int _last_frame = recv[4]; 
	int _task_size = recv[5]; 
	int _priority = recv[6]; 
	bool _suspend = recv[7]; 
	string _comment = recv[8];
	string _software = recv[9];
	string _project = recv[10]; 
	string _extra = recv[11]; 
	string _system = recv[12];  
	int _instances = recv[13]; 
	string _render = recv[14];

	string status, submit_start;

	if ( _suspend ){ status = "Suspended"; }
	else { status = "Queue"; }

	submit_start = currentDateTime(0);

	// checkea si el nombre esta en la lista, si esta le pone un padding
	string job_name = _job_name;

    for (int i = 0; i < 100; ++i){
		bool inside = false;
		for ( auto j : jobs){
			if ( job_name == j->name ){
				inside = true;
			}
		}
		if ( inside ){
			job_name = _job_name + "_" + to_string(i);
		}
		else { break; }
	}
	//----------------------------------------------------------------------

	auto tasks = make_task( _first_frame, _last_frame, _task_size );

	job_struct *_job = new job_struct;

	_job->name = job_name;
	_job->status = status;
	_job->priority = _priority;
	_job->server = _server;
	_job->server_group = _server_group;
	_job->instances = _instances;
	_job->comment = _comment;
	_job->submit_start = submit_start;
	_job->submit_finish = "...";
	_job->timer = "...";
	_job->timer2 = "...";
	_job->total_render_time = "...";
	_job->estimated_time = "...";
	_job->timer_last_active = 0;	
	_job->software = _software;
	_job->project = _project;
	_job->system = _system;
	_job->extra = _extra;
	_job->render = _render;
	_job->vetoed_servers = {};
	_job->progres = 0;
	_job->old_p = 0;
	_job->waiting_task = tasks.size();
	_job->tasks = tasks.size();
	_job->suspended_task = 0;
	_job->failed_task = 0;
	_job->active_task = 0;
	_job->task_size = _task_size;
	_job->task = tasks;
	_job->first_frame = _first_frame;
	_job->last_frame = _last_frame;

	jobs.push_back(_job);

	return {};
}

vector <task_struct*> manager::make_task( int first_frame, int last_frame, int task_size ){

	//Crea una lista de tareas con el frame de inicio y final
	vector <int> range;
	vector <vector <int>> tasks_range;

	int t = task_size - 1;
	int f = first_frame - task_size;
	int s, l;

	while (1){
		f=f+t+1;
		for (int i=0; i<10000; i++){
			s=f+t;				
			l=f+i;
			if ( l==s or l==last_frame ){ break; }
		}

		range = {f,l};
		tasks_range.push_back(range);

		if ( l == last_frame ){ break; }
	}
	//--------------------------------------------------------

	// create tasks
	vector <task_struct*> tasks;

	int num=0;
	string task_name;
	for ( auto i : tasks_range ){
		num++;

		if ( num < 10 ) task_name = "Task_00" + to_string(num);
		else if ( num > 99 ) task_name = "Task_" + to_string(num);
		else task_name = "Task_0" + to_string(num);

		task_struct *_tasks = new task_struct;

		_tasks->name=task_name;
		_tasks->status="waiting";
		_tasks->first_frame=i[0];
		_tasks->last_frame=i[1];
		_tasks->server="...";
		_tasks->time="...";

		tasks.push_back(_tasks);
	}
	//--------------------------------

	return tasks;
}

// Envia  informacion de jobs al monitor
json manager::sendToMonitor_thread( json recv ){
	return struct_to_json();
} //-----------------------------------------

void manager::json_to_struct( json info ){
	debug("manager::json_to_struct.");
	for ( auto job : info[ "jobs" ] ){
		job_struct *_jobs = new job_struct;
		_jobs->name = job[ "name" ];
		_jobs->status = job[ "status" ];
		_jobs->priority = job[ "priority" ];
		for ( string _server : job[ "server" ] ) _jobs->server.push_back( _server );
		for ( string _group : job[ "server_group" ] ) _jobs->server_group.push_back( _group );
		_jobs->instances = job[ "instances" ];
		_jobs->comment = job[ "comment" ];
		_jobs->submit_start = job[ "submit_start" ];
		_jobs->submit_finish = job[ "submit_finish" ];
		_jobs->timer = job[ "timer" ];
		_jobs->timer2 = job[ "timer2" ];
		_jobs->total_render_time = job[ "total_render_time" ];
		_jobs->estimated_time = job[ "estimated_time" ];
		_jobs->timer_last_active = job[ "timer_last_active" ];
		_jobs->software = job[ "software" ];
		_jobs->project = job[ "project" ];
		_jobs->system = job[ "system" ];
		_jobs->extra = job[ "extra" ];
		_jobs->render = job[ "render" ];

		for ( auto vs : job[ "vetoed_servers" ] ){
			_jobs->vetoed_servers.push_back( vs );
		}

		_jobs->progres = job[ "progres" ];
		_jobs->old_p = job[ "old_p" ];
		_jobs->waiting_task = job[ "waiting_task" ];
		_jobs->tasks = job[ "tasks" ];
		_jobs->suspended_task = job[ "suspended_task" ];
		_jobs->failed_task = job[ "failed_task" ];
		_jobs->active_task = job[ "active_task" ];
		_jobs->task_size = job[ "task_size" ];
		_jobs->first_frame = job[ "first_frame" ];
		_jobs->last_frame = job[ "last_frame" ];
		
		for ( auto t : job[ "task" ] ){
			task_struct *_tasks = new task_struct; 
			_tasks->name = t[ "name" ];  
			_tasks->status = t[ "status" ];  
			_tasks->first_frame = t[ "first_frame" ];  
			_tasks->last_frame = t[ "last_frame" ];  
			_tasks->server = t[ "server" ];  
			_tasks->time = t[ "time" ];  
			_jobs->task.push_back( _tasks );
		}

		jobs.push_back( _jobs );

	}

	for ( auto server : info[ "servers" ] ){
		server_struct *_server = new server_struct;
		_server->name = server["name"];
		_server->status = server["status"];
		_server->host = server["host"];
		_server->system = server["system"];
		_server->cpu = server["cpu"];
		_server->cpu_cores = server["cpu_cores"];
		_server->ram = server["ram"];
		_server->ram_used = server["ram_used"];
		_server->ram_total = server["ram_total"];
		_server->temp = server["temp"];
		_server->vbox = server["vbox"];
		_server->response_time = server["response_time"];

		for ( auto ins : server["instances"] ){
			inst_struct *_ins = new inst_struct;
			_ins->index = ins[0];
			_ins->status = ins[1];
			_ins->reset = ins[2];
			_ins->job_task = ins[3];

			_server->instances.push_back( _ins ); 
		}

		_server->max_instances = server["max_instances"];
		_server->sshUser = server["sshUser"];
		_server->sshPass = server["sshPass"];
		_server->vmSoftware = server["vmSoftware"];
		_server->schedule = server["schedule"];
		_server->schedule_state_0 = server["schedule_state_0"];
		_server->schedule_state_1 = server["schedule_state_1"];

		servers.push_back( _server );
	}

	for ( auto group : info[ "groups" ] ){
		group_struct *_group = new group_struct;
		_group->name = group[ "name" ];
		_group->status = group[ "status" ];
		_group->totaMachine = group[ "totaMachine" ];
		_group->activeMachine = group[ "activeMachine" ];

		for ( auto server : group["server"] ){
			serverFromGroupStruct *_server = new serverFromGroupStruct;
			_server->name = server[0];
			_server->status = server[1];
			_group->server.push_back( _server );
		}

		groups.push_back( _group );

	}
}

QJsonObject manager::struct_to_json(){

	// combierte todas las estructuras de Jobs y las combierte a JSON para poder enviarlas y guardarlas
	QJsonObject info;
	info[ "jobs" ];

	for ( auto job : jobs ){	
		auto& i = info[ "jobs" ][ job->name ];

		i[ "name" ] = job->name;
		i[ "status" ] = job->status;
		i[ "priority" ] = job->priority;
		for ( string _server : job->server ) i[ "server" ].push_back( _server );
		for ( string _group : job->server_group ) i[ "server_group" ].push_back( _group );
		i[ "instances" ] = job->instances;
		i[ "comment" ] = job->comment;
		i[ "submit_start" ] = job->submit_start;
		i[ "submit_finish" ] = job->submit_finish;
		i[ "timer" ] = job->timer;
		i[ "timer2" ] = job->timer2;
		i[ "total_render_time" ] = job->total_render_time;
		i[ "estimated_time" ] = job->estimated_time;
		i[ "timer_last_active" ] = job->timer_last_active;
		i[ "software" ] = job->software;
		i[ "project" ] = job->project;
		i[ "system" ] = job->system;
		i[ "extra" ] = job->extra;
		i[ "vetoed_servers" ] = job->vetoed_servers;
		i[ "render" ] = job->render;
		i[ "progres" ] = job->progres;
		i[ "old_p" ] = job->old_p;
		i[ "waiting_task" ] = job->waiting_task;
		i[ "tasks" ] = job->tasks;
		i[ "suspended_task" ] = job->suspended_task;
		i[ "failed_task" ] = job->failed_task;
		i[ "active_task" ] = job->active_task;
		i[ "task_size" ] = job->task_size;
		i[ "first_frame" ] = job->first_frame;
		i[ "last_frame" ] = job->last_frame;
		
		for ( auto task : job->task ){
			json t;
			t[ "name" ] = task->name ;  
			t[ "status" ] = task->status ;  
			t[ "first_frame" ] = task->first_frame ;  
			t[ "last_frame" ] = task->last_frame ;  
			t[ "server" ] = task->server ;  
			t[ "time" ] = task->time ;

		    i[ "task" ].push_back(t);

		}
	}
	//------------------------------------------------------------------------------

	info[ "servers" ];
	for ( auto server : servers ){
		auto& s = info[ "servers" ][ server->name ];
		s[ "name" ] = server->name;
		s[ "status" ] = server->status;
		s[ "host" ] = server->host;
		s[ "system" ] = server->system;
		s[ "cpu" ] = server->cpu;
		s[ "cpu_cores" ] = server->cpu_cores;
		s[ "ram" ] = server->ram;
		s[ "ram_used" ] = server->ram_used;
		s[ "ram_total" ] = server->ram_total;
		s[ "temp" ] = server->temp;
		s[ "vbox" ] = server->vbox;
		s[ "response_time" ] = server->response_time;
		for ( auto instance : server->instances ){
			s[ "instances" ].push_back( { instance->index, instance->status, instance->reset, instance->job_task } );  
		}
		s[ "max_instances" ] = server->max_instances;
		s[ "sshUser" ] = server->sshUser;
		s[ "sshPass" ] = server->sshPass;
		s[ "vmSoftware" ] = server->vmSoftware;
		s[ "schedule" ] = server->schedule;
		s[ "schedule_state_0" ] = server->schedule_state_0;
		s[ "schedule_state_1" ] = server->schedule_state_1;
		s[ "log" ] = server->log;

	}

	info[ "groups" ];
	for ( auto group : groups ){
		auto& g = info[ "groups" ][ group->name ];
		g[ "name" ] = group->name;
		g[ "status" ] = group->status;
		g[ "totaMachine" ] = group->totaMachine;
		g[ "activeMachine" ] = group->activeMachine;
		for ( auto server : group->server ){
            g[ "server" ].push_back( { server->name, server->status } );
		}
	}

	return info;
}

