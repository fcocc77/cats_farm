#ifndef SHARED_VARIABLES_H
#define SHARED_VARIABLES_H

#include <vector>
using namespace std;
#include <QJsonArray>


struct shared_variables{
    QJsonArray server_display;
    QJsonArray jobs;
    QJsonArray servers;
    bool stopUpdate;

};

#endif //SHARED_VARIABLES_H