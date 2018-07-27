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

	string system;
	if ( _linux ){ system="Linux"; }
	else if ( _win32 ) system="Windows";
	else { system="Mac"; }

	string log = fread( "../../log/render_log_0" );

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
						 log
						};

	return server_info;
}

json server::recieveManager( json recv, int input ){

	debug("server::recieveManager.");
	string send;

	if ( input == 0 ) send = _render->render_task( recv );

	if ( input == 1 ) send = fread( "../../log/server_log" );

	if ( input == 2 ){
		string log_file;
		if ( recv == "failed" ){ log_file = "../../log/render_log"; }
		else{ log_file = "../../log/render_log_0"; }

		// Lee el log render
		string log = fread( log_file );
		string called_log = fread( "../../log/called_log" );

		send = called_log + "\n\n" + log;
	}

	if ( input == 3 ){

		for ( int i : recv ){
			_render->taskKill[i] = true;
			os::kill( _render->pid[i] );
		}

	}

	if ( input == 4 ){

		json action = recv[0]; json info = recv[1];

		if ( action == "freeram" ){
			if ( _linux ){
				os::sh( "sync && sysctl -w vm.drop_caches=3" );
			}	
		}

		if ( action == "vmstart" ){ 
			string vm;
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

			string vm;
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

