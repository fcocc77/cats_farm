#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
using namespace std;
#include <thread>

#include "../utils/util.h"
#include "../utils/tcp.h"

#include <QCoreApplication>
#include <QProcess>
#include <QObject>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonArray>

class logger : public QObject
{
  public:
    logger();
    const QString managerHost = fread(path + "/etc/manager_host");

    QString get(QString recv);
};

#endif // LOGGER_H