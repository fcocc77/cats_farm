#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
using namespace std;
#include <thread>

#include "../utils/util.hpp"
#include "../utils/tcp.hpp"

#include <QCoreApplication>
#include <QProcess>
#include <QObject>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonArray>

#include "render.hpp"

class server : public QObject
{
public:
	render *_render = new render();

	void init();
	QString send_resources(QString recv, QJsonObject extra);
	QString recieveManager(QString data);

	const QString managerHost = fread(path + "/etc/manager_host");
};

#endif // SERVER_HPP