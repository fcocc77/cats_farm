// Author: Francisco Jose Contreras Cuevas
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#ifndef GENERAL_H
#define GENERAL_H

#include <QMainWindow>
#include <QObject>
#include <QSystemTrayIcon>

#include "shared_variables.h"

class general_class : public QObject
{
private:
    QMainWindow *_monitor;
    shared_variables *shared;

    QSystemTrayIcon *notify;
    QStringList completed_jobs;
    QWidget *properties;

    // General Funcions
    void setup_ui();
    void connections();
    void notify_icon();
    void update_style(bool from_source_code = false);

public:
    general_class(QMainWindow *_monitor, shared_variables *_shared,
                  QWidget *_properties);

    // General Action
    QAction *settings_action;
    QAction *quit_action;
    QAction *hide_action;
    QAction *show_action;
    QAction *properties_hide;
    QAction *update_style_action;
};

#endif // GENERAL_H
