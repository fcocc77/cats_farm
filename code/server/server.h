#ifndef SERVER_H
#define SERVER_H

#include <iostream>
using namespace std;
#include <vector>
#include <thread>

#include "../utils/util.h"
//#include "../manager/structs.h"

#include "../utils/json.h"
#include "../utils/denoiser.h"

using namespace nlohmann;
#include "../utils/tcp.h"

#include <QApplication>
#include <QProcess>
#include <QObject>
#include <QFileInfo>



class server : public QObject{
public:

	string path;

	server( string exe );




	string render_task( json recv );
	string post_software( int total_frame, string log_file, string software);
	string pre_software(string software, string project, int first_frame, int last_frame,
					string extra, int instance, string src_path, 
					string dst_path, string jobSystem, string render, string log_file );
	json send_resources( json recv );
	bool vbox_working();
	json recieveManager( json data, int input );


	


	vector <bool> taskKill = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	vector <bool> renderInstance = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	vector <int> pid = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	json preferences;
	const string managerHost = fread( "../../etc/manager_host" );



};







#endif // SERVER_H