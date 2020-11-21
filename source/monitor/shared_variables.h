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
    QComboBox *zone_box;
    QLabel *conection;
    QLabel *jobs_count;
    QTreeWidget *tasks_tree;
};

#endif //SHARED_VARIABLES_HPP