#include "submit.h"

void submit::init( int argc, char const *argv[] ){

	QString arg, jobName, server, serverGroup, firstFrame, lastFrame, taskSize, priority, 
						suspend, comment, software, project, extra, system, instances, render;

	for ( int i=1; i < argc; i++){
		arg = argv[i];
		if ( arg == "-jobName" ){ jobName = argv[i+1]; }
		if ( arg == "-server" ){ server = argv[i+1]; }
		if ( arg == "-serverGroup" ){ serverGroup = argv[i+1]; }
		if ( arg == "-firstFrame" ){ firstFrame = argv[i+1]; }
		if ( arg == "-lastFrame" ){ lastFrame = argv[i+1]; }
		if ( arg == "-taskSize" ){ taskSize = argv[i+1]; }
		if ( arg == "-priority" ){ priority = argv[i+1]; }
		if ( arg == "-suspend" ){ suspend = argv[i+1]; }
		if ( arg == "-comment" ){ comment = argv[i+1]; }
		if ( arg == "-software" ){ software = argv[i+1]; }
		if ( arg == "-project" ){ project = argv[i+1]; }
		if ( arg == "-extra" ){ extra = argv[i+1]; }
		if ( arg == "-system" ){ system = argv[i+1]; }
		if ( arg == "-instances" ){ instances = argv[i+1]; }
		if ( arg == "-render" ){ render = argv[i+1]; }

	}

	bool _suspend = false;
	if ( suspend == "1" ) _suspend = true;

	auto toInt = [=]( QString str ){
		try{ return stoi(str); }
		catch( exception& e ){ return 0; }
	};

	QJsonArray info = { jobName, server, serverGroup, toInt(firstFrame), toInt(lastFrame), toInt(taskSize), toInt(priority), 
					_suspend, comment, software, project, extra, system, toInt(instances), render };

	QString path = os::dirname(os::dirname(os::dirname(argv[0])));
	QString managerHost = fread( path + "/etc/manager_host" );

	if ( argc > 2 )
		tcpClient( managerHost, 7000, info, 0 );
}

