#include "jobs_actions.h"

void jobs_actions::init(){

    actions();
}

void jobs_actions::actions(){

	// signal del drop a la lista
	//connect( jobsList, &QTreeWidget::itemDropped, this, submitDrop );
	//------------------------------------

	connect( jobsList, &QTreeWidget::itemDoubleClicked, this, &jobs_actions::jobModify );
	connect( jobsList, &QTreeWidget::customContextMenuRequested, this, &jobs_actions::job_popup );

	// Job Acciones
	connect( deleteAction, &QAction::triggered, this, &jobs_actions::itemDelete );

	deleteAction->setIcon( QIcon("../../icons/delete.png") );
	deleteAction->setShortcut( QString("Del"));

	connect( jobSuspendAction, &QAction::triggered, this,[this](){
		QString ask = "Sure you want to suspend the job?";
		QString tile = "Job Suspend";
		QString action = "suspend";
		jobMessage( &jobs_actions::jobAction, action, ask,  tile, this );
	});

	connect( jobRestartAction, &QAction::triggered, this,[this](){
		QString ask = "Sure you want to restart the job?";
		QString tile = "Job Restart";
		QString action = "restart";
		jobMessage( &jobs_actions::jobAction, action, ask,  tile, this );
	});

	jobSuspendAction->setIcon(QIcon("../../icons/pause.png"));

	connect( jobResumeAction, &QAction::triggered, this, [this](){
		jobAction("resume");
	});
	jobResumeAction->setIcon(QIcon("../../icons/play3.png"));

	connect( jobUnlockServersAction, &QAction::triggered, this, [this](){
		jobAction("unlock");		
	});

	connect( jobLogAction, &QAction::triggered, this, &jobs_actions::jobShowLog);
	jobLogAction->setShortcut( QString("L") );

	connect( jobModifyAction, &QAction::triggered, this, &jobs_actions::jobModify);
	jobModifyAction->setShortcut( QString("M") );

	connect( uiJobOptions->dialog, &QDialogButtonBox::accepted, this, &jobs_actions::jobOptionsOk );	
	connect( uiJobOptions->dialog, &QDialogButtonBox::rejected, [this](){
		uiJobOptions->hide();
	});
	//-----------------------------------------------------------------------
}

void jobs_actions::job_popup(){

	auto selected = jobsList->selectedItems();
	if ( not selected.empty() ){

		QMenu *menu = new QMenu( monitor );

		menu->addAction( jobSuspendAction );			
		menu->addAction( jobResumeAction );
		menu->addAction( deleteAction );
		menu->addAction( jobRestartAction );

		menu->addSeparator();
		menu->addAction( jobLogAction );
		menu->addAction( jobUnlockServersAction );

		menu->addSeparator();

		menu->addAction( jobShowRenderFolderAction );
		menu->addAction( jobModifyAction );

		menu->popup( QCursor::pos() );
	}
}

void jobs_actions::jobShowLog(){

	auto selected = jobsList->selectedItems();
	if ( not selected.empty() ){

		QString job_name = selected[0]->text(0);

		// encuentra job seleccionado en la jobs recividos
		QJsonObject job;
		for ( QJsonValue j : shared->jobs ){
			QJsonObject _job = j.toObject();
			if ( _job["name"].toString() == job_name ) 
				job = _job;
		}
		//--------------------------------------

		for ( QJsonValue t : job["task"].toArray() ){
			QJsonObject task = t.toObject();
			log_server = task["server"].toString();
			log_server = log_server.split( ":" )[0];

			if ( not ( log_server == "...") ){
  
				QStringList vetoed_host;
				QStringList vetoed_name;
				QStringList all_host;
				QStringList all_name;

				for (int i = 0; i < serverList->topLevelItemCount(); ++i) {
					auto item = serverList->topLevelItem(i);
					QString name = item->text(0);
					QString _host = item->text(7);

					for ( QJsonValue vs : job["vetoed_servers"].toArray() ){ 
						if ( name == vs.toString() ) {
							vetoed_host.push_back( _host );
							vetoed_name.push_back( name );
						}
					}
					if ( name == log_server ){ 
						all_host.push_back( _host );
						all_name.push_back( name );
					}
				}

				QString _name;
				QString _host;
				bool failed;
				if ( not all_host.empty() ) {
					_host = all_host[0];
					_name = all_name[0];
					failed = false;
				}
				if ( not vetoed_host.empty() ) {
					_host = vetoed_host[0];
					_name = vetoed_name[0];
					failed = true;
				}

				QString result = tcpClient( _host, 7001, jats({ 1, failed }) );

				log_text->setPlainText( _name + " Log:\n\n" + result );
				break;

			}
			else log_text->setPlainText( "The jobs has not yet rendered" );
		}

		log_dock->show();
	}
}

void jobs_actions::jobModify(){
	uiJobOptions->show();

	// Recive los servers del jobs que estan en el manager
	auto selected = jobsList->selectedItems();
	firstJobItem = selected[0];

	if ( not selected.empty() ){

		QString job_name = selected.takeLast()->text(0);

		QJsonArray send = {{ job_name, "options", "read" }};
		send = { send, "jobOptions" };
		QString recv = tcpClient( managerHost, 7000, jats({ 3, send }) );
		QJsonArray pks = jafs( recv );
		//-----------------------------------------

		QStringList serverExist; 
		for ( QJsonValue s : pks[0].toArray() ) 
			serverExist.push_back( s.toString() );

		vector <QString> serverGroupExist; 
		for ( QJsonValue sg : pks[1].toArray() ) 
			serverGroupExist.push_back( sg.toString() );

		int priority = pks[2].toInt(); 
		QString comment = pks[3].toString();
		int instances = pks[4].toInt();
		int task_size = pks[5].toInt();
		QString _job_name = pks[6].toString();
		int first_frame = pks[7].toInt();
		int last_frame = pks[8].toInt();

		uiJobOptions->priority->setCurrentIndex( priority );
		uiJobOptions->firstFrame->setText( QString::number( first_frame ) );
		uiJobOptions->lastFrame->setText( QString::number( last_frame ) );
		uiJobOptions->jobName->setText( _job_name );
		uiJobOptions->taskSize->setText( QString::number( task_size ) );
		uiJobOptions->comment->setText( comment );
		uiJobOptions->instances->setText( QString::number( instances ) );

		QString jobServerAsignName = job_name;

		uiJobOptions->serverAsign->setSelectionMode( QAbstractItemView::ExtendedSelection ); 
		uiJobOptions->serverAsign->setIndentation(0);     
		uiJobOptions->serverAsign->setColumnWidth(0,100);
		uiJobOptions->serverAsign->setColumnWidth(1,50);

		uiJobOptions->serverGroupAsign->setSelectionMode( QAbstractItemView::ExtendedSelection ); 
		uiJobOptions->serverGroupAsign->setIndentation(0);     
		uiJobOptions->serverGroupAsign->setColumnWidth(0,100);
		uiJobOptions->serverGroupAsign->setColumnWidth(1,50);

		uiJobOptions->serverAsign->clear();
		for (int i = 0; i < serverList->topLevelItemCount(); ++i){

			auto _item = serverList->topLevelItem(i); 
			QString name = _item->text(0);

			QTreeWidgetItem *item = new QTreeWidgetItem();

			item->setText(0,name);

			item->setCheckState( 0, Qt::Unchecked );

			if ( serverExist.contains( name ) ){
				item->setCheckState(0, Qt::Checked );
			}

			uiJobOptions->serverAsign->addTopLevelItem(item);
		}

		uiJobOptions->serverGroupAsign->clear();
		for (int i = 0; i < groupList->topLevelItemCount(); ++i){

			auto _item = groupList->topLevelItem(i); 
			QString name = _item->text(2);

			QTreeWidgetItem *item = new QTreeWidgetItem();

			item->setText(0,name);

			item->setCheckState( 0, Qt::Unchecked );
			if ( in_vector( name, serverGroupExist ) ){
				item->setCheckState(0, Qt::Checked );
			}
			uiJobOptions->serverGroupAsign->addTopLevelItem( item );
		}
	}
}

void jobs_actions::jobOptionsOk(){

	//Obtiene servideros chekeados para el job seleccionado
	QJsonArray machines;
	for (int i = 0; i < uiJobOptions->serverAsign->topLevelItemCount(); ++i){
		auto item = uiJobOptions->serverAsign->topLevelItem(i); 
		QString name = item->text(0);

		if ( item->checkState(0) ){
			machines.push_back(name);
		}
	}
	//--------------------------------------------

	//Obtiene grupos chekeados para el job seleccionado
	QJsonArray group;
	for (int i = 0; i < uiJobOptions->serverGroupAsign->topLevelItemCount(); ++i){
		auto item = uiJobOptions->serverGroupAsign->topLevelItem(i); 
		QString name = item->text(0);

		if ( item->checkState(0) ){
			group.push_back(name);
		}
	}
	//--------------------------------------------

	int priority = uiJobOptions->priority->currentIndex();
	int first_frame = uiJobOptions->firstFrame->text().toInt();
	int last_frame = uiJobOptions->lastFrame->text().toInt();
	int task_size = uiJobOptions->taskSize->text().toInt();
	QString comment = uiJobOptions->comment->text();
	QString _job_name = uiJobOptions->jobName->text();

	int instances = uiJobOptions->instances->text().toInt();
	if ( instances > 16 ){
		instances = 16;
	}

	QJsonArray pks;
	auto selected = jobsList->selectedItems();
	selected.push_front( firstJobItem );

	QStringList repeatItem;
	for ( auto item : selected ){
		QString job_name = item->text(0);
		QJsonArray options = { machines, group, priority, comment, instances, first_frame, last_frame, task_size, _job_name };
		// el primer item se repite asi que si esta 2 veces no lo agrega otra vez
		if ( not repeatItem.contains( job_name ) )
			pks.push_back( {{ job_name, options, "write" }} );
		repeatItem.push_back( job_name );
		//-------------------------------------------------------
	}
	pks = { pks, "jobOptions" };

	QString ask = "Sure you want to send the changes? \nSome frames will be lost due to the size of the tasks.";

	QMessageBox::StandardButton reply;
	reply = QMessageBox::question( monitor, "Job Options", ask, QMessageBox::Yes | QMessageBox::No );
	if (reply == QMessageBox::Yes) {
		tcpClient( managerHost, 7000, jats({ 3, pks }) );
		uiJobOptions->hide();
	}
}

void jobs_actions::jobMessage( void ( jobs_actions::*funtion )( QString ), QString action, 
						 QString ask, QString tile, jobs_actions *_class ){

	auto selected = jobsList->selectedItems();

	if ( not selected.empty() ){

		QMessageBox::StandardButton reply;
		reply = QMessageBox::question( monitor, tile, ask, QMessageBox::Yes | QMessageBox::No );
		if (reply == QMessageBox::Yes) {
			( _class->*funtion )( action );
		}

	}
}

void jobs_actions::jobDeleteStart( QString action ){

	auto root = jobsList->invisibleRootItem();
	auto selected = jobsList->selectedItems();

	QJsonArray pks;

	for ( auto item : selected ){

		QString job_name = item->text(0);
		os::remove( "../../log/trayIcon/" + job_name );

		pks.push_back( {{ job_name, action }} );

		root->removeChild( item );

	}

	pks = { pks, "jobAction" };

	tcpClient( managerHost, 7000, jats({ 3, pks }) );
}

void jobs_actions::jobAction( QString action ){
	shared->stopUpdate = true;

	auto selected = jobsList->selectedItems();

	QJsonArray pks;
	for ( auto item : selected ){
		QString job_name = item->text(0);
		pks.push_back( {{ job_name, action }} ); 
	}
	pks = { pks, "jobAction" };

	tcpClient( managerHost, 7000, jats({ 3, pks }) );	
}

void jobs_actions::itemDelete(){

	shared->stopUpdate = true;

	QString ask = "Sure you want to delete the job?";
	QString tile = "Job Delete";
	QString action = "delete";
	jobMessage( &jobs_actions::jobDeleteStart, action, ask,  tile, this );
}