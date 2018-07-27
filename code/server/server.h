#ifndef SERVER_H
#define SERVER_H

#include <iostream>
using namespace std;
#include <vector>
#include <thread>

#include "../utils/util.h"
#include "../utils/json.h"

using namespace nlohmann;
#include "../utils/tcp.h"

#include <QApplication>
#include <QProcess>
#include <QObject>
#include <QFileInfo>

#include "render.h"

class server : public QObject{
public:

	render *_render = new render();

	void init();
	json send_resources( json recv );
	json recieveManager( json data, int input );

	const string managerHost = fread( "../../etc/manager_host" );

};

#endif // SERVER_H