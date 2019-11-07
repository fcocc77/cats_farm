#ifndef SHARED_VARIABLES_HPP
#define SHARED_VARIABLES_HPP

using namespace std;
#include <QJsonArray>
#include <QJsonObject>

struct shared_variables
{
    QJsonObject server_display;
    QJsonObject jobs;
    QJsonObject settings;
    QJsonArray servers;
    bool stopUpdate;
    bool app_close;
    QString manager_host;
    int manager_port;
    int server_port;
};

#endif //SHARED_VARIABLES_HPP