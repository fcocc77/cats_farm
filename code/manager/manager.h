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
#include <QJsonObject>
#include <QJsonArray>

#include "structs.h"
#include "../utils/video.h"
#include "../utils/util.h"
#include "../utils/tcp.h"
#include "../utils/os.h"
#include "../utils/threading.h"

class manager : public QObject{
public:
	vector <job_struct*> jobs;
	vector <server_struct*> servers;
	vector <group_struct*> groups;
	QJsonObject preferences = jread( "../../etc/preferences.json" );
	bool reset_render;

	void init();
	QJsonObject make_job( QJsonObject recv );
	vector <task_struct*> make_task( int first_frame, int last_frame, int task_size );
	void resetAllServer();
	void kill_tasks( job_struct *job, bool _delete );
	void render_job();
	void render_task( server_struct *server, inst_struct *instance, job_struct *job );
	QJsonObject sendToMonitor_thread( QJsonObject recv );
	QJsonObject struct_to_json();
	void json_to_struct( QJsonObject info );
    void reactive_all();
	QJsonObject recieve_monitor_thread( QJsonObject recv );
	void jobAction( QJsonObject pks );
	QJsonObject jobOptions( QJsonObject pks );
	QJsonObject serverAction( QJsonObject pks );
	QJsonObject serverOptions( QJsonObject pks );
	void groupAction( QJsonObject pks );
	void taskAction( QJsonObject pks );
	void groupCreate( QJsonObject pks );
	QJsonObject preferencesAction( QJsonObject pks );
	QJsonObject jobLogAction( QJsonObject pks );
    void serverSetState( server_struct *server, bool state );
	QJsonObject server_tcp( QJsonObject recv, int input );

    QJsonObject update_server_thread( QJsonObject recv );
    void update_server();
    bool iTime( QString schedule );
    void update_group();
    void update_jobs();
	void container_save();
	void update_all();

	// encuentra un index por el nombre de un vector de estructuras
	template < typename T > // para que el retorno sea diferente al primer argumento
	auto *find_struct( T lista, QString name ){
		for ( auto s : lista ){
			if ( s->name == name ){ return s; }
		}
		return lista[0];
	}
	//-----------------------------------------------------------------

	template < typename T > 
    bool is_struct( T lista, QString name ){
		for ( auto s : lista ){
			if ( s->name == name ){ return true; }
		}
		return false;
    }

	void erase_by_name( auto& lista, QString name ){
		int i = 0;
		for ( auto s : lista ){
			if ( s->name == name ){ break; }
			i++;
		}

		lista.erase( lista.begin() + i, lista.begin() + i+1 );
	}
};

#endif //MANAGER_H