#ifndef SERVERS_ACTIONS_H
#define SERVERS_ACTIONS_H

// QT5 Library
#include <QString>
#include <QList>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QSignalMapper>
#include <QMessageBox>
#include <QObject>
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QDockWidget>
//---------------------

#include "../utils/util.h"
#include "../utils/tcp.h"
#include "../utils/json.h"
using namespace nlohmann;
#include <iostream>
using namespace std;

// monitor modules
#include "ui_jobs_tree.h"
#include "ui_servers_tree.h"
#include "ui_groups_tree.h"
#include "ui_server_options.h"

//-----------------

class servers_actions : public QObject{
public:
    QMainWindow *monitor;
    ui_jobs_tree *jobsList;
    ui_servers_tree *serverList;
    ui_groups_tree *groupList;
    ui_server_options *uiServerOptions;
    QPlainTextEdit *log_text;
    QDockWidget *log_dock;


    template < class T >
    servers_actions( T *_monitor  ){
        monitor = _monitor;
        jobsList = _monitor->jobsList;
        serverList = _monitor->serverList;
        groupList = _monitor->groupList;
        uiServerOptions = _monitor->uiServerOptions;
        log_text = _monitor->log_text;
        log_dock = _monitor->log_dock;
        init();
    }

	QTreeWidgetItem *firstServerItem;

    const string managerHost = fread( "../../etc/manager_host" );

	// Server Action
	QAction *serverInactiveAction = new QAction("Disable");
	QAction *serverReactiveAction = new QAction("Enable");
	QAction *serverMaxInstancesAction = new QAction("Max Instances");
	QAction *serverFreeramAction = new QAction("Free Ram");
	QAction *serverSshAction = new QAction("SSH");
	QAction *serverShowLog = new QAction("Show Log");
	QAction *serverVncAction = new QAction("VNC");
	QAction *serverStartVmAction = new QAction("Start VirtualMachine");
	QAction *serverStopVmAction = new QAction("Stop VirtualMachine");
	QAction *deleteAction = new QAction("Delete Server");

	//------------------------------------------------

	// Server Funcions
    void init();
    void actions();
	void serverOptions();
	void serverOptionsSend();
	void server_popup();
	void serverCpuLimit(int limit );
	void serverLog();
	void serverMaxInstances( int ins );
	void serverSSH();
	void serverVNC();
	void serverMessage( json ( servers_actions::*funtion )( string, string ), string action, 
								QString ask, QString tile, string info, servers_actions *_class );	
	void serverActionMessage();
	json serverAction( string action, string info );
	void sendToServer( string action, string info );

	//----------------

};

#endif //SERVERS_ACTIONS_H