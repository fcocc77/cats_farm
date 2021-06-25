#ifndef GENERAL_H
#define GENERAL_H

#include "includes.h"

class general_class : public QObject
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
    void update_style(bool from_source_code = false);
    //----------------------------

public:
    general_class(QMainWindow *_monitor, shared_variables *_shared,
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

#endif // GENERAL_H
