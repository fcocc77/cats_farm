#ifndef SHARED_VARIABLES_HPP
#define SHARED_VARIABLES_HPP

using namespace std;
#include <QJsonArray>
#include <QJsonObject>

struct shared_variables
{
    QJsonObject server_display;
    QJsonObject jobs;
    QJsonArray servers;
    bool stopUpdate;
    bool app_close;
};

#endif //SHARED_VARIABLES_HPP