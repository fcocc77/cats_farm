#ifndef MANAGER_H
#define MANAGER_H

#include <iostream>
using namespace std;
#include <vector>
#include <ctime> // time_t
#include <algorithm> //sort
#include <unistd.h>// sleep usleep
#include <thread>   // thread
#include <QApplication>
#include <QString>
#include <QDebug>
#include <QObject>

#include "structs.h"
#include "../utils/video.h"
#include "../utils/util.h"
#include "../utils/tcp.h"
#include "../utils/os.h"
#include "../utils/json.h"
#include "../utils/threading.h"
using namespace nlohmann;

class manager : public QObject{
public:
	vector <job_struct*> jobs;
	vector <server_struct*> servers;
	vector <group_struct*> groups;
	json preferences = jread( "../../etc/preferences.json" );
	bool reset_render;


	void init();
	json make_job( json recv );
	vector <task_struct*> make_task( int first_frame, int last_frame, int task_size );
	void resetAllServer();
	void kill_tasks( job_struct *job, bool _delete );
	void render_job();
	void render_task( server_struct *server, inst_struct *instance, job_struct *job );
	json sendToMonitor_thread( json recv );
	json struct_to_json();
	void json_to_struct( json info );
    void reactive_all();
	json recieve_monitor_thread( json recv );
	void jobAction( json pks );
	json jobOptions( json pks );
	json serverAction( json pks );
	json serverOptions( json pks );
	void groupAction( json pks );
	void taskAction( json pks );
	void groupCreate( json pks );
	json preferencesAction( json pks );
	json jobLogAction( json pks );
    void serverSetState( server_struct *server, bool state );
	json server_tcp( json recv, int input );

    json update_server_thread( json recv );
    void update_server();
    bool iTime( string schedule );
    void update_group();
    void update_jobs();
	void container_save();
	void update_all();	    
	

	// encuentra un index por el nombre de un vector de estructuras
	template < typename T > // para que el retorno sea diferente al primer argumento
	auto *find_struct( T lista, string name ){
		for ( auto s : lista ){
			if ( s->name == name ){ return s; }
		}
		return lista[0];
	}
	//-----------------------------------------------------------------
	
	template < typename T > 
    bool is_struct( T lista, string name ){
		for ( auto s : lista ){
			if ( s->name == name ){ return true; }
		}
		return false;
    }


	void erase_by_name( auto& lista, string name ){
		int i = 0;
		for ( auto s : lista ){
			if ( s->name == name ){ break; }
			i++;
		}

		lista.erase( lista.begin() + i, lista.begin() + i+1 );
	}

	
};

#endif //MANAGER_H