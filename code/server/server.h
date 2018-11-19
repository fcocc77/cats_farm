#ifndef SERVER_H
#define SERVER_H

#include <iostream>
using namespace std;
#include <vector>
#include <thread>

#include "../utils/util.h"
#include "../utils/tcp.h"

#include <QApplication>
#include <QProcess>
#include <QObject>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonArray>


#include "render.h"

class server : public QObject{
public:

	render *_render = new render();

	void init();
	QString send_resources( QString recv );
	QString recieveManager( QString data );

	const QString managerHost = fread( "../../etc/manager_host" );

};

#endif // SERVER_H