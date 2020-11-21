#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
using namespace std;
#include <thread>

#include <QCoreApplication>
#include <QProcess>
#include <QObject>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonArray>
#include <QMutex>

#include <util.h>
#include <os.h>
#include "../utils/tcp.h"
#include "../utils/threading.h"

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