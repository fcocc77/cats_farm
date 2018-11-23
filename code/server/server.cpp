#include "server.h"

void server::init(){
    tcpClient( managerHost, 7000, &server::send_resources, this );
	tcpServer( 7001, &server::recieveManager, this );
}

QString server::send_resources( QString recv ){

	if ( not recv.isEmpty() ){
		_render->preferences = jofs( recv );
		jwrite( "../../etc/preferences_s.json", _render->preferences );
	}

	QString system;
	if ( _linux ){ system="Linux"; }
	else if ( _win32 ) system="Windows";
	else { system="Mac"; }

	//get ssh user
	bool usr = false;
	static QString username;
	static QString userpass;

	if ( not usr ){ 
		if ( _win32 ){ 
			username = os::sh("echo %username%").split(" ")[0];
			userpass = fread( "C:/ProgramData/cats_farm/user" );
		}
		else{
			username = "root";
			userpass = "vfx";
		}
		usr = true;
	} //------------------------------------------

	QJsonArray server_info = { os::hostName(),
						 os::ip(),
						 os::cpuUsed(),
						 os::ramPercent(),
						 os::cpuTemp(),
						 system,
						 _render->vbox_working(),
						 os::ramTotal(),
						 os::ramUsed(),
						 os::cpuCount(),
						 "log",
						 username,
						 userpass
						};

	return jats( { 1, server_info } );
}

QString server::recieveManager( QString _recv ){

	QJsonArray json = jafs( _recv );
	int input = json[0].toInt();
	QJsonArray recv = json[1].toArray();

	QString send;

	if ( input == 0 ) send = _render->render_task( recv );

	if ( input == 1 ) {
		bool failed = recv[0].toBool();
		if ( failed ) send = fread( "../../log/render_log" );
		else send = fread( "../../log/render_log_0" );
	}

	if ( input == 3 ){

		for ( auto i : recv ){
			// kill cinema 4d que esta dentro de la maquina virtual
			if ( _render->VMCinemaActive ) {
				QString VMCinemaKill = _render->VMSH + "\"taskkill\" \"-im\" \"Cinema 4D.exe\" \"-f\"";
				_render->qprocess( VMCinemaKill );
			} //-------------------------------------------------------------------------

			_render->taskKill[i.toInt()] = true;
			os::kill( _render->pid[i.toInt()] );
		}

		// kill noice
		if ( _linux ) _render->qprocess( "pkill noice" );
		else  _render->qprocess( "taskkill -im -f noice.exe" );
		//---------------------------
	}

	if ( input == 4 ){

		QString action = recv[0].toString(); 
		QString info = recv[1].toString();

		if ( action == "freeram" ){
			if ( _linux ){
				os::sh( "sync && sysctl -w vm.drop_caches=3" );
			}	
		}

		if ( action == "vmstart" ){ 
			QString vm;
			if ( _linux ){
				vm = "'VBoxManage startvm win2016 --type headless'";
				vm = "runuser -l " + os::user() +" -c " + vm;
			}
			else if ( _win32 ){
				vm = "\"C:/Program Files/Oracle/VirtualBox/VBoxManage.exe\" startvm win2016 --type headless";
			}

			os::sh( vm );
		 }

		if ( action == "vmstop" ){

			QString vm;
			if ( _linux ){
				vm = "'VBoxManage controlvm win2016 savestate'";
				vm = "runuser -l " + os::user() + " -c " + vm;
			}
			else if ( _win32 ){
				vm="\"C:/Program Files/Oracle/VirtualBox/VBoxManage.exe\" controlvm win2016 savestate";
			}

			os::sh( vm );
			fwrite( "../../log/vbox","0" );

		}

		if ( action == "cpu_limit" ){
			fwrite( "../../etc/cpu_limit", info );
		}
	}

	return send;
}

