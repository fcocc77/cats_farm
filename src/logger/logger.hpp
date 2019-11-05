#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
using namespace std;
#include <thread>

#include "../utils/util.hpp"
#include "../utils/tcp.hpp"
#include "../utils/os.hpp"
#include "../utils/threading.hpp"

#include <QCoreApplication>
#include <QProcess>
#include <QObject>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonArray>
#include <QMutex>

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