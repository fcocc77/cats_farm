#ifndef TOOLBAR_HPP
#define TOOLBAR_HPP

#include "includes.hpp"

#include "global.hpp"
#include "jobs.hpp"
#include "groups.hpp"
#include "servers.hpp"
#include "tasks.hpp"
#include "log.hpp"
#include "update.hpp"
#include "shared_variables.hpp"
#include "properties.hpp"

class toolbar_class : public QWidget
{
private:
    QMainWindow *monitor;
    global_class *global;
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

    QPushButton *jobs_switch;
    QPushButton *tasks_switch;
    QPushButton *groups_switch;
    QPushButton *servers_switch;
    QPushButton *log_switch;

public:
    toolbar_class(
        QMainWindow *_monitor,
        global_class *_global,
        jobs_class *_jobs,
        tasks_class *_tasks,
        servers_class *_servers,
        groups_class *_groups,
        log_class *_log,
        update_class *_update,
        shared_variables *_shared,
        QWidget *_settings_widget,
        properties_class *_properties);
    ~toolbar_class();
};

#endif //TOOLBAR_HPP
