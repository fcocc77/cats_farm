#ifndef TASKS_ACTIONS_H
#define TASKS_ACTIONS_H

// QT5 Library
#include <QAction>
#include <QObject>
#include <QMainWindow> 
#include <QDebug>
//----------

#include <iostream>
using namespace std;

#include "ui_tasks_tree.h"
#include "ui_jobs_tree.h"

class tasks_actions : public QObject{
public:

    ui_jobs_tree *jobsList;
	ui_tasks_tree *taskList;
    QMainWindow *monitor;

    const string managerHost = fread( "../../etc/manager_host" );

    template < class T >
    tasks_actions( T *_monitor ){

    	monitor = _monitor;
    	taskList = _monitor->taskList;
        jobsList = _monitor->jobsList;

    	acionts();
    }

	// Task Action
	QAction *taskSuspendAction = new QAction("Suspend");
	QAction *taskRestartAction = new QAction("Restart");
	//------------------------------------------------

	// Task Funtions
	void task_popup();
	void taskRestart();
	void taskMessage( string action, QString ask, QString tile );
	void taskAction( string action );
    void acionts();

	//-----------------

};

#endif //TASKS_ACTIONS_H