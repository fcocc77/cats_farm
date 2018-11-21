#include "tasks_actions.h"

// Task Funtions

void tasks_actions::acionts(){

	connect( taskList, &QTreeWidget::customContextMenuRequested, this, &tasks_actions::task_popup );

	// Task Action
	connect( taskSuspendAction, &QAction::triggered, this, [this] (){
		taskAction( "suspend" );} 
	);
	connect( taskRestartAction, &QAction::triggered, this, &tasks_actions::taskRestart );

	connect( taskRenderServerAction, &QAction::triggered, this, &tasks_actions::taskRenderServer );

	//-----------------------------------------------------
}

void tasks_actions::task_popup(){
	auto selected = taskList->selectedItems();
	if ( not selected.empty() ){ 
		QMenu *menu = new QMenu( monitor );

		menu->addAction( taskSuspendAction );
		menu->addSeparator();
		menu->addAction( taskRestartAction );
		menu->addAction( taskRenderServerAction );
		menu->popup( QCursor::pos() );

	}
}

void tasks_actions::taskRestart(){
	QString ask = "Are you Sure you want to restart the task?";
	QString tile = "Task Restart";
	QString action = "resume";

	taskMessage( action, ask, tile );
}

void tasks_actions::taskRenderServer(){
	auto selected = taskList->selectedItems();
	QString _server =  selected[0]->text( 3 );
	_server = _server.split( ":" )[0];

	for (int i = 0; i <  taskList->topLevelItemCount(); ++i){
		auto item = taskList->topLevelItem(i);

		QString server =  item->text( 3 );
		server = server.split( ":" )[0];
		if ( _server == server ) item->setSelected(true);
	}

}

void tasks_actions::taskMessage( QString action, QString ask, QString tile ){

	auto selected = taskList->selectedItems();
	if ( not selected.empty() ){

		QMessageBox::StandardButton reply;
		reply = QMessageBox::question( monitor, tile, ask, QMessageBox::Yes | QMessageBox::No );
		if (reply == QMessageBox::Yes) {
			taskAction( action );
		}

	}
}

void tasks_actions::taskAction( QString action ){

	QJsonArray pks;
	for ( auto item_job : jobsList->selectedItems() ){
		QString job_name = item_job->text(0);

		for ( auto item_task : taskList->selectedItems() ){ 
			QString task_name = item_task->text(0);
			pks.push_back( {{ job_name, task_name, action }} );
		}
	}

	pks = { pks, "taskAction" };

	tcpClient( managerHost, 7000, jats({ 3, pks }) );
}

