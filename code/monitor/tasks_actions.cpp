#include "tasks_actions.h"

// Task Funtions

void tasks_actions::acionts(){

	connect( taskList, &QTreeWidget::customContextMenuRequested, this, &tasks_actions::task_popup );

	// Task Action
	connect( taskSuspendAction, &QAction::triggered, this, [this] (){
		taskAction( "suspend" );} 
	);
	connect( taskRestartAction, &QAction::triggered, this, &tasks_actions::taskRestart );
	//-----------------------------------------------------
}

void tasks_actions::task_popup(){
	auto selected = taskList->selectedItems();
	if ( not selected.empty() ){ 
		QMenu *menu = new QMenu( monitor );

		menu->addAction( taskSuspendAction );
		menu->addSeparator();
		menu->addAction( taskRestartAction );
		menu->popup( QCursor::pos() );

	}
}

void tasks_actions::taskRestart(){
	QString ask = "Are you Sure you want to restart the task?";
	QString tile = "Task Restart";
	string action = "resume";

	taskMessage( action, ask, tile );
}

void tasks_actions::taskMessage( string action, QString ask, QString tile ){

	auto selected = taskList->selectedItems();
	if ( not selected.empty() ){

		QMessageBox::StandardButton reply;
		reply = QMessageBox::question( monitor, tile, ask, QMessageBox::Yes | QMessageBox::No );
		if (reply == QMessageBox::Yes) {
			taskAction( action );
		}

	}
}

void tasks_actions::taskAction( string action ){

	json pks;
	for ( auto item_job : jobsList->selectedItems() ){
		string job_name = item_job->text(0).toStdString();

		for ( auto item_task : taskList->selectedItems() ){ 
			string task_name = item_task->text(0).toStdString();
			pks.push_back( { job_name, task_name, action } );
		}
	}

	pks = { pks, "taskAction" };

	tcpClient( managerHost, 7000, pks, 3 );
}

