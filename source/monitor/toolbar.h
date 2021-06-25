#ifndef TOOLBAR_HPP
#define TOOLBAR_HPP

#include <QWidget>

#include "general.h"
#include "groups.h"
#include "jobs.h"
#include "log.h"
#include "properties.h"
#include "servers.h"
#include "shared_variables.h"
#include "tasks.h"
#include "update.h"

class toolbar_class : public QWidget
{
private:
    QMainWindow *monitor;
    general_class *global;
    jobs_class *jobs;
    tasks_class *tasks;
    servers_class *servers;
    groups_class *groups;
    log_class *log;
    update_class *update;
    shared_variables *shared;
    QWidget *settings_widget;
    properties_class *properties;

    void setup_ui();
    void connections();
    void load_zones();

    QPushButton *resume;
    QPushButton *suspend;
    QPushButton *settings;
    QPushButton *submit;

    QPushButton *jobs_switch;
    QPushButton *tasks_switch;
    QPushButton *groups_switch;
    QPushButton *servers_switch;
    QPushButton *log_switch;

public:
    toolbar_class(QMainWindow *_monitor, general_class *_global,
                  jobs_class *_jobs, tasks_class *_tasks,
                  servers_class *_servers, groups_class *_groups,
                  log_class *_log, update_class *_update,
                  shared_variables *_shared, QWidget *_settings_widget,
                  properties_class *_properties);
    ~toolbar_class();
};

#endif // TOOLBAR_HPP
