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

QJsonObject manager::server_tcp( QJsonObject pks, int input ){

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

QJsonObject manager::make_job( QJsonObject recv ){

	string _job_name = recv[0].toString(); 
	//------------------------------
	vector <string> _server;
	if ( recv[1].toString() != "None" ) _server.push_back( recv[1].toString().toStdString() );
	//------------------------------
	vector <string> _server_group;
	if ( recv[2].toString() != "None" ) _server_group.push_back( recv[2].toString().toStdString() );
	//------------------------------
	int _first_frame = recv[3].toInt(); 
	int _last_frame = recv[4].toInt(); 
	int _task_size = recv[5].toInt(); 
	int _priority = recv[6].toInt(); 
	bool _suspend = recv[7].toBool(); 
	string _comment = recv[8].toString();
	string _software = recv[9].toString();
	string _project = recv[10].toString(); 
	string _extra = recv[11].toString(); 
	string _system = recv[12].toString();  
	int _instances = recv[13].toInt(); 
	string _render = recv[14].toString();

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
QJsonObject manager::sendToMonitor_thread( QJsonObject recv ){
	return struct_to_json();
} //-----------------------------------------

void manager::json_to_struct( QJsonObject info ){

	for ( auto job : info[ "jobs" ].toObject() ){
		job_struct *_jobs = new job_struct;
		_jobs->name = job[ "name" ].toString();
		_jobs->status = job[ "status" ].toString();
		_jobs->priority = job[ "priority" ].toInt();
		for ( string _server : job[ "server" ].toArray() ) 
			_jobs->server.push_back( _server.toString() );
		for ( string _group : job[ "server_group" ].toArray() ) 
			_jobs->server_group.push_back( _group.toString() );
		_jobs->instances = job[ "instances" ].toInt();
		_jobs->comment = job[ "comment" ].toString();
		_jobs->submit_start = job[ "submit_start" ].toString();
		_jobs->submit_finish = job[ "submit_finish" ].toString();
		_jobs->timer = job[ "timer" ].toString();
		_jobs->timer2 = job[ "timer2" ].toString();
		_jobs->total_render_time = job[ "total_render_time" ].toString();
		_jobs->estimated_time = job[ "estimated_time" ].toString();
		_jobs->timer_last_active = job[ "timer_last_active" ].toBool();
		_jobs->software = job[ "software" ].toString();
		_jobs->project = job[ "project" ].toString();
		_jobs->system = job[ "system" ].toString();
		_jobs->extra = job[ "extra" ].toString();
		_jobs->render = job[ "render" ].toString();

		for ( auto vs : job[ "vetoed_servers" ].toArray() ){
			_jobs->vetoed_servers.push_back( vs.toString() );
		}

		_jobs->progres = job[ "progres" ].toInt();
		_jobs->old_p = job[ "old_p" ].toInt();
		_jobs->waiting_task = job[ "waiting_task" ].toInt();
		_jobs->tasks = job[ "tasks" ].toInt();
		_jobs->suspended_task = job[ "suspended_task" ].toInt();
		_jobs->failed_task = job[ "failed_task" ].toInt();
		_jobs->active_task = job[ "active_task" ].toInt();
		_jobs->task_size = job[ "task_size" ].toInt();
		_jobs->first_frame = job[ "first_frame" ].toInt();
		_jobs->last_frame = job[ "last_frame" ].toInt();

		for ( auto t : job[ "task" ].toArray ){
			task_struct *_tasks = new task_struct; 
			_tasks->name = t[ "name" ].toString();  
			_tasks->status = t[ "status" ].toString();  
			_tasks->first_frame = t[ "first_frame" ].toInt();  
			_tasks->last_frame = t[ "last_frame" ].toInt();  
			_tasks->server = t[ "server" ].toString();  
			_tasks->time = t[ "time" ].toString();  
			_jobs->task.push_back( _tasks );
		}

		jobs.push_back( _jobs );

	}

	for ( auto server : info[ "servers" ].toObject() ){
		server_struct *_server = new server_struct;
		_server->name = server["name"].toString();
		_server->status = server["status"].toString();
		_server->host = server["host"].toString();
		_server->system = server["system"].toString();
		_server->cpu = server["cpu"].toInt();
		_server->cpu_cores = server["cpu_cores"].toInt();
		_server->ram = server["ram"].toInt();
		_server->ram_used = server["ram_used"].toDoble();
		_server->ram_total = server["ram_total"].toInt();
		_server->temp = server["temp"].toInt();
		_server->vbox = server["vbox"].toBool();
		_server->response_time = server["response_time"].toInt();

		for ( auto ins : server["instances"].toArray() ){
			inst_struct *_ins = new inst_struct;
			_ins->index = ins[0].toInt();
			_ins->status = ins[1].toInt();
			_ins->reset = ins[2].toBool();
			_ins->job_task = ins[3].toString();

			_server->instances.push_back( _ins ); 
		}

		_server->max_instances = server["max_instances"].toInt();
		_server->sshUser = server["sshUser"].toString();
		_server->sshPass = server["sshPass"].toString();
		_server->vmSoftware = server["vmSoftware"].toString();
		_server->schedule = server["schedule"].toString();
		_server->schedule_state_0 = server["schedule_state_0"].toBool();
		_server->schedule_state_1 = server["schedule_state_1"].toBool();

		servers.push_back( _server );
	}

	for ( auto group : info[ "groups" ].toObject() ){
		group_struct *_group = new group_struct;
		_group->name = group[ "name" ].toString();
		_group->status = group[ "status" ].toBool();
		_group->totaMachine = group[ "totaMachine" ].toInt();
		_group->activeMachine = group[ "activeMachine" ].toInt();

		for ( auto server : group["server"].toArray() ){
			serverFromGroupStruct *_server = new serverFromGroupStruct;
			_server->name = server[0].toString();
			_server->status = server[1].toBool();
			_group->server.push_back( _server );
		}

		groups.push_back( _group );

	}
}

QJsonObject manager::struct_to_json(){

	// combierte todas las estructuras de Jobs y las combierte a JSON para poder enviarlas y guardarlas
	QJsonObject info;

	QJsonObject _jobs;
	for ( auto job : jobs ){	
		QJsonObject j;

		j[ "name" ] = job->name;
		j[ "status" ] = job->status;
		j[ "priority" ] = job->priority;
		// --------------------------------
		QJsonArray serverList;
		for ( string _server : job->server ) 
			serverList.push_back( _server );
		j[ "server" ] = serverList;
		// --------------------------------
		QJsonArray serverGroup;
		for ( string _group : job->server_group )
			serverGroup.push_back( _group );
		j[ "server_group" ] = serverGroup;
		// --------------------------------
		j[ "instances" ] = job->instances;
		j[ "comment" ] = job->comment;
		j[ "submit_start" ] = job->submit_start;
		j[ "submit_finish" ] = job->submit_finish;
		j[ "timer" ] = job->timer;
		j[ "timer2" ] = job->timer2;
		j[ "total_render_time" ] = job->total_render_time;
		j[ "estimated_time" ] = job->estimated_time;
		j[ "timer_last_active" ] = job->timer_last_active;
		j[ "software" ] = job->software;
		j[ "project" ] = job->project;
		j[ "system" ] = job->system;
		j[ "extra" ] = job->extra;
		j[ "vetoed_servers" ] = job->vetoed_servers;
		j[ "render" ] = job->render;
		j[ "progres" ] = job->progres;
		j[ "old_p" ] = job->old_p;
		j[ "waiting_task" ] = job->waiting_task;
		j[ "tasks" ] = job->tasks;
		j[ "suspended_task" ] = job->suspended_task;
		j[ "failed_task" ] = job->failed_task;
		j[ "active_task" ] = job->active_task;
		j[ "task_size" ] = job->task_size;
		j[ "first_frame" ] = job->first_frame;
		j[ "last_frame" ] = job->last_frame;

		QJsonArray _tasks;
		for ( auto task : job->task ){
			QJsonObject t;
			t[ "name" ] = task->name ;  
			t[ "status" ] = task->status ;  
			t[ "first_frame" ] = task->first_frame ;  
			t[ "last_frame" ] = task->last_frame ;  
			t[ "server" ] = task->server ;  
			t[ "time" ] = task->time ;

		    _tasks.push_back( t );
		}
		j[ "task" ] = _tasks;

		_jobs[ job->name ] = j;

	}

	info[ "jobs" ] = _jobs;
	//------------------------------------------------------------------------------

	QJsonObject _servers;
	for ( auto server : servers ){
		QJsonObject s;
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
		// --------------------------------
		QJsonArray _instances;
		for ( auto instance : server->instances ){
			_instances.push_back( { instance->index, instance->status, instance->reset, instance->job_task } );  
		}
		s[ "instances" ] = _instances;
		// --------------------------------
		s[ "max_instances" ] = server->max_instances;
		s[ "sshUser" ] = server->sshUser;
		s[ "sshPass" ] = server->sshPass;
		s[ "vmSoftware" ] = server->vmSoftware;
		s[ "schedule" ] = server->schedule;
		s[ "schedule_state_0" ] = server->schedule_state_0;
		s[ "schedule_state_1" ] = server->schedule_state_1;
		s[ "log" ] = server->log;

		_servers[ server->name ] = s;

	}

	info[ "servers" ] = _servers;
	//------------------------------------------------------------------------------

	QJsonObject _groups;

	for ( auto group : groups ){
		QJsonObject g;
		g[ "name" ] = group->name;
		g[ "status" ] = group->status;
		g[ "totaMachine" ] = group->totaMachine;
		g[ "activeMachine" ] = group->activeMachine;
		QJsonArray serverList;
		for ( auto server : group->server ){
            serverList.push_back( { server->name, server->status } );
		}
		g[ "server" ] = serverList;

		_groups[ group->name ] = g;
	}

	info[ "groups" ] = _groups;

	return info;
}

