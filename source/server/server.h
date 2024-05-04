// Author: Francisco Contreras
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
using namespace std;
#include <thread>

#include "../utils/tcp.h"
#include <util.h>

#include <QCoreApplication>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <QMutex>
#include <QObject>
#include <QProcess>

#include "render.h"

class server : public QObject
{
private:
    render_class *render;
    QMutex mutex;

    QString send_resources(QString recv, QJsonObject extra);
    QString update_from_manager(QString data);

    QString get_log() const;

public:
    server();
};

#endif // SERVER_HPP
