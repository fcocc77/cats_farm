#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
using namespace std;
#include <thread>

#include "../utils/util.hpp"
#include "../utils/tcp.hpp"
#include "../utils/os.hpp"

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

#endif // LOGGER_HPP