#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
using namespace std;
#include <thread>

#include "../utils/util.h"
#include "../utils/tcp.h"
#include "../utils/os.h"

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
    QJsonObject zones;
    QString get(QString recv, QJsonObject extra);
};

#endif // LOGGER_H