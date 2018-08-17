#ifndef JOBS_ACTIONS_H
#define JOBS_ACTIONS_H

// QT5 Library
#include <QMenu>
#include <QString>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QDockWidget>
#include <QTreeWidgetItem>
#include <QDialogButtonBox>
#include <QObject>
#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QWidget>
#include <QDebug>
#include <QIcon>
#include <QThread>
//-------------------

#include <iostream>
using namespace std;

#include "../utils/threading.h"
#include "../utils/util.h"
#include "../utils/tcp.h"
#include "../utils/json.h"
using namespace nlohmann;

// monitor modules
#include "ui_jobs_tree.h"
#include "ui_servers_tree.h"
#include "ui_groups_tree.h"
#include "ui_job_options.h"
#include "get_manager_info.h"
#include "shared_variables.h"
#include "../external/QCodeEditor/QCodeEditor_Example.hpp"
//-----------------

class jobs_actions : public QObject{
public:
	QMainWindow *monitor;
	ui_jobs_tree *jobsList;
	ui_servers_tree *serverList;
	ui_groups_tree *groupList;
	ui_job_options *uiJobOptions;
	get_manager_info *getManagerInfo;

	QThread *log_thread = new QThread();

	kgl::QCodeEditor_Example *log_text;
	QDockWidget *log_dock;
	string log_server;
	shared_variables *shared;

	template < class T >
	jobs_actions( T *_monitor  ){
		monitor = _monitor;
		jobsList = _monitor->jobsList;
		serverList = _monitor->serverList;
		groupList = _monitor->groupList;
		uiJobOptions = _monitor->uiJobOptions;
		getManagerInfo = _monitor->getManagerInfo;
		log_text = _monitor->log_text;
		log_dock = _monitor->log_dock;
		shared = _monitor->shared;
		init();
	}

	//-------------------
	QTreeWidgetItem *firstJobItem;

	// variables Generales
	const string managerHost = fread( "../../etc/manager_host" );
	//--------------------------------------

	// Job Acciones
	QAction *deleteAction = new QAction( "Delete" );
	QAction *jobSuspendAction = new QAction("Suspend");
	QAction *jobResumeAction = new QAction("Resume");
	QAction *jobRestartAction = new QAction("Restart");
	QAction *jobUnlockServersAction = new QAction("Enable blocked servers");
	QAction *jobLogAction = new QAction("Show Log");
	QAction *jobModifyAction = new QAction("Modify");
	QAction *jobShowRenderFolderAction = new QAction("Show Render Folder");
	//------------------------------------------------

	// Jobs Funcions
	void init();
	void actions();
	void job_popup();
	void jobShowLog();
	void jobModify();
	void jobOptionsOk();

	void jobDeleteStart( string action );   
	void jobAction( string action );

	void jobMessage( void ( jobs_actions::*funtion )( string ), string action, 
							QString ask, QString tile, jobs_actions *_class );

	void itemDelete();

	//-----------------------------

};

#endif //JOBS_ACTIONS_H