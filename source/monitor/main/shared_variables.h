// Author: Francisco Jose Contreras Cuevas
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#ifndef SHARED_VARIABLES_HPP
#define SHARED_VARIABLES_HPP

#include <QJsonArray>
#include <QJsonObject>
#include <QComboBox>
#include <QLabel>
#include <QTreeWidget>

#include "combo_box.h"

struct shared_variables
{
    QJsonObject server_display;
    QJsonObject jobs;
    QJsonObject settings;
    QJsonObject servers;
    bool stopUpdate;
    bool app_close;
    QString manager_host;
    combo_box *zone_box;
    QLabel *conection;
    QLabel *jobs_count;
    QTreeWidget *tasks_tree;
};

#endif // SHARED_VARIABLES_HPP
