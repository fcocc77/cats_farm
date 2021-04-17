#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
using namespace std;
#include <thread>

#include <QCoreApplication>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <QMutex>
#include <QObject>
#include <QProcess>

#include "../utils/tcp.h"
#include "../utils/threading.h"
#include <os.h>
#include <util.h>

class logger : public QObject
{
public:
    logger();
    QJsonObject zones;
    QJsonObject zones_jobs;
    QMutex mutex;
    QString get(QString recv, QJsonObject extra);
    void save();
};

#endif // LOGGER_HPP
