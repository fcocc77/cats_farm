#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include "includes.h"

class global_class : public QObject
{
private:
    shared_variables *shared;
    QMainWindow *monitor;

    QSystemTrayIcon *notify;
    QStringList completed_jobs;
    QWidget *properties;

    // General Funcions
    void setup_ui();
    void connections();
    void notify_icon();
    void style();
    //----------------------------

public:
    global_class(QMainWindow *_monitor, shared_variables *_shared,
                 QWidget *_properties);

    // General Action
    QAction *preferences_action;
    QAction *quit_action;
    QAction *hide_action;
    QAction *show_action;
    QAction *properties_hide;
    QAction *update_style_action;
    //------------------------------------------------
};

#endif // GLOBAL_HPP
