#include "server.h"

void server::init(){
    tcpClient( managerHost, 7000, &server::send_resources, this, 1 );
	tcpServer( 7001, &server::recieveManager, this );
}

json server::send_resources( json recv ){

	if ( not recv.empty() ){
		_render->preferences = recv;
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
			username = split( os::sh("echo %username%"), " " )[0];
			userpass = fread( "C:/ProgramData/cats_farm/user" );
		}
		else{
			username = "root";
			userpass = "vfx";
		}
		usr = true;
	} //------------------------------------------

	json server_info = { os::hostName(),
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

	return server_info;
}

json server::recieveManager( json recv, int input ){

	debug("server::recieveManager.");
	QString send;

	if ( input == 0 ) send = _render->render_task( recv );

	if ( input == 1 ) {
		bool failed = recv;
		if ( failed ) send = fread( "../../log/render_log" );
		else send = fread( "../../log/render_log_0" );
	}

	if ( input == 3 ){

		for ( int i : recv ){
			// kill cinema 4d que esta dentro de la maquina virtual
			if ( _render->VMCinemaActive ) {
				QString VMCinemaKill = _render->VMSH + "\"taskkill\" \"-im\" \"Cinema 4D.exe\" \"-f\"";
				_render->qprocess( VMCinemaKill );
			} //-------------------------------------------------------------------------

			_render->taskKill[i] = true;
			os::kill( _render->pid[i] );
		}

		// kill noice
		if ( _linux ) _render->qprocess( "pkill noice" );
		else  _render->qprocess( "taskkill -im -f noice.exe" );
		//---------------------------
	}

	if ( input == 4 ){

		json action = recv[0]; json info = recv[1];

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

	json _send;
	_send = send;

	return _send;
}

