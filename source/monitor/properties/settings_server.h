// Author: Francisco Jose Contreras Cuevas
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#ifndef SETTINGS_SERVER_H
#define SETTINGS_SERVER_H

#include <QWidget>

#include "combo_box.h"
#include "shared_variables.h"

class settings_server : public QWidget
{
private:
    shared_variables *shared;
    QWidget *settings_widget;

    combo_box *manager_host;
    QString current_ip;
    void update_ips();

public:
    settings_server(shared_variables *shared, QWidget *settings_widget);

    void save();
    void restore();
};
#endif // SETTINGS_SERVER_H
