#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
using namespace std;
#include <thread>

#include <util.h>
#include "../utils/tcp.h"

#include <QCoreApplication>
#include <QProcess>
#include <QObject>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonArray>
#include <QMutex>

#include "render.h"

class server : public QObject
{
public:
	render_class *render;
	QMutex mutex;

	server();
	QString send_resources(QString recv, QJsonObject extra);
	QString recieveManager(QString data);
};

#endif // SERVER_HPP