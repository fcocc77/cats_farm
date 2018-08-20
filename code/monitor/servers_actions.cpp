#include "servers_actions.h"

// Server Funcions

void servers_actions::init(){
    actions();
}

void servers_actions::actions(){

    // Server List Connections

	connect( serverList, &QTreeWidget::customContextMenuRequested, this, &servers_actions::server_popup );

	// Server Action

	connect( deleteAction, &QAction::triggered, this, [this](){
		QString ask = "Are you sure to want delete this server?";
		QString tile = "Server Delete";
		string action = "delete";

		serverMessage( &servers_actions::serverAction, action, ask, tile, "None", this );
	});

	connect( serverInactiveAction, &QAction::triggered, this, [this](){
		QString ask = "The task will kill, Sure you want to disable this server?";
		QString tile = "Server Disabling";
		string action = "inactive";
		serverMessage( &servers_actions::serverAction, action, ask, tile, "None", this );
	});
	connect( serverReactiveAction, &QAction::triggered, this, [this](){
		serverAction( "reactive","None" );
	});
	connect( serverMaxInstancesAction, &QAction::triggered, this, &servers_actions::serverMaxInstances );
	connect( serverFreeramAction, &QAction::triggered, this,[this](){
		sendToServer("freeram","none");
	});
	connect( serverSshAction, &QAction::triggered, this, &servers_actions::serverSSH );
	connect( serverShowLog, &QAction::triggered, this, &servers_actions::serverLog );
	serverShowLog->setShortcut( QString( "Ctrl+L"));
	connect( serverVncAction, &QAction::triggered, this, &servers_actions::serverVNC );
	connect( serverStartVmAction, &QAction::triggered, this, [this](){
		sendToServer("vmstart","none");
	});
	connect( serverStopVmAction, &QAction::triggered, this, [this](){
		sendToServer("vmstop","none");
	});
	//--------------------------------------------------------
}

void servers_actions::serverOptions(){

	uiServerOptions->show();

	// Recive los servers del jobs que estan en el manager

	QList < QTreeWidgetItem* > selected = serverList->selectedItems();

	firstServerItem = selected[0];

	if ( not selected.empty() ){

		json pks = { ( selected[0]->text(0) ).toStdString() , "None", "read" };
		pks = { pks, "serverOptions" };
		json recv = tcpClient( managerHost, 7000, pks, 3 );
		//vmSoftware, schedule = recv

		string schedule = recv;

		auto range = split( schedule, "-" );

		uiServerOptions->schedule_start->setCurrentIndex( stoi( range[0] ) );
		uiServerOptions->schedule_end->setCurrentIndex( stoi( range[1] ) );

	}
}

void servers_actions::serverOptionsSend(){

	string schedule = uiServerOptions->schedule_start->currentIndex() + "-" + uiServerOptions->schedule_end->currentIndex();

	json pks;
	auto selected = serverList->selectedItems();
	selected.push_back( firstServerItem );

	for ( auto item : selected ){
		string server = item->text(0).toStdString();
		pks.push_back( { server, schedule, "write" } );
	}

	pks = { pks, "serverOptions"};
	tcpClient( managerHost, 7000, pks, 3 );
}

void servers_actions::server_popup(){
	auto selected = serverList->selectedItems();

    QMenu *menu = new QMenu( monitor );
	if ( not selected.empty() ){

		menu->addAction( serverInactiveAction );
		menu->addAction( serverReactiveAction );
		menu->addAction( deleteAction );

		menu->addSeparator();
		menu->addAction( serverShowLog );
		menu->addSeparator();

		/*
		//-------------------------------------------------
		QMenu *submenu = new QMenu("Max Instances");			
		QSignalMapper *mapper = new QSignalMapper();

		for (int i = 1; i < 17; ++i){
			QString n = QString::number(i);
			QAction *action = new QAction( n + " Instances" );
			connect( action, SIGNAL("triggered()"), mapper, SLOT("map()"));
			mapper->setMapping( action, n );
			submenu->addAction( action );			
		}
		connect(mapper, SIGNAL(mapped(int)), this, SLOT(serverMaxInstances(int)));
		menu->addMenu( submenu );
		//-------------------------------------------------

		QMenu *submenu2 = new QMenu("CPU Limit");			
		QSignalMapper *mapper2 = new QSignalMapper();

		for (int i = 1; i < 11; ++i){
			QString n = QString::number( i * 10 );
			QAction *action = new QAction( n + "% CPU" );
			connect( action, SIGNAL("triggered()"), mapper, SLOT("map()"));
			mapper2->setMapping( action, n );
			submenu2->addAction( action );			
		}
		connect(mapper2, SIGNAL(mapped(int)), this, SLOT(serverCpuLimit(int)));
		menu->addMenu( submenu2 );
		//---------------------------------------------------
		*/
		//menu->addMenu( submenu );
		menu->addAction( serverFreeramAction );
		menu->addSeparator();
		menu->addAction( serverStartVmAction );
		menu->addAction( serverStopVmAction );
		menu->addSeparator();
		menu->addAction( serverSshAction );
		menu->addAction( serverVncAction );
		menu->addSeparator();
		//menu->addAction( groupCreateAction );
		menu->popup( QCursor::pos() );
	}
	else{

		menu->addAction( serverList->showAllAction );
		menu->addAction( serverList->hideAllAction );
		menu->addSeparator();
		menu->addAction( serverList->displayOnAction );			
		menu->addAction( serverList->displayOffAction );
		menu->addSeparator();
		menu->addAction( serverList->displayWindowsAction );
		menu->addAction( serverList->displayLinuxAction );
		menu->addAction( serverList->displayMacAction );
		menu->popup( QCursor::pos() );
	}
}

void servers_actions::serverCpuLimit( int limit ){
	sendToServer( "cpu_limit", to_string(limit) );
}

void servers_actions::serverLog(){

	auto selected = serverList->selectedItems();
	if ( not selected.empty() ){ 
		string host = ( selected[0]->text(7) ).toStdString();

		string result;
		result = tcpClient( host, 7001, true, 1 );

		log_text->setPlainText( QString::fromStdString(result));
		log_dock->show();
	}
}

void servers_actions::serverMaxInstances( int ins ){
	serverAction("max_instances", to_string(ins));
}

void servers_actions::serverSSH(){

	json recv = serverAction("ssh","None");

	string sshUser = recv[0], 
	sshPass = recv[1], 
	ip = recv[2];

	string disable_ask = " -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no ";
	string cmd;
	string ssh = "\"C:/Program Files/OpenSSH/bin/ssh.exe\"";
	if ( _win32 ) cmd = "cmd /C " + ssh + disable_ask + sshUser + "@" + ip;
	else cmd = "gnome-terminal -e 'sshpass -p " + sshPass + " ssh -o StrictHostKeyChecking=no " + sshUser + "@" + ip + "'";

	os::back( cmd );
}

void servers_actions::serverVNC(){

	string tigervnc;
	if ( _linux ){ tigervnc = "vncviewer"; }
	else if ( _win32 ){ tigervnc = path() + "/os/win/vnc/vncviewer.exe"; }
	else{ tigervnc = path() + "/os/mac/vnc/vncviewer"; }

	for ( auto item : serverList->selectedItems() ){
		string ip = item->text(7).toStdString();
		string arg = "\"" + tigervnc + "\" " + ip + ":1"; 
		os::back( arg );
	}
}

void servers_actions::serverMessage( json ( servers_actions::*funtion )( string, string ), string action, 
								QString ask, QString tile, string info, servers_actions *_class ){

	auto selected = serverList->selectedItems();
	if ( not selected.empty() ){

		QMessageBox::StandardButton reply;
		reply = QMessageBox::question( monitor, tile, ask, QMessageBox::Yes | QMessageBox::No );
		if (reply == QMessageBox::Yes) {
			( _class->*funtion )( action, info);
		}

	}
}

void servers_actions::serverActionMessage(){
}

json servers_actions::serverAction( string action, string info ){

	json pks;
	auto selected = serverList->selectedItems();
	for ( auto item : selected ){
		string server_name = item->text(0).toStdString();
		if ( not item->parent() ){
			pks.push_back( {server_name, action, info } );
		}

	}
	pks = { pks, "serverAction" };

	json recv = tcpClient( managerHost, 7000, pks, 3 );

	return recv;
}

void servers_actions::sendToServer( string action, string info ){

	QList <QTreeWidgetItem*> selected = serverList->selectedItems();

	for ( auto item : selected ){
		string host = item->text(7).toStdString();
		tcpClient( host, 7001, { action, info }, 4);
	}
	//-------------------------------------
}
//----------------------------------
