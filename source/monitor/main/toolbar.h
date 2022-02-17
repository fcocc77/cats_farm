// Author: Francisco Jose Contreras Cuevas
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#ifndef TOOLBAR_HPP
#define TOOLBAR_HPP

#include <QWidget>

#include "general.h"
#include "groups_widget.h"
#include "jobs_widget.h"
#include "log.h"
#include "properties.h"
#include "servers_widget.h"
#include "shared_variables.h"
#include "tasks_widget.h"
#include "update.h"
#include "button.h"

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

    button *settings;
    button *submit;
    button *options_button;

    button *tasks_switch;
    button *groups_switch;
    button *servers_switch;
    button *log_switch;

    void setup_ui();
    void connections();
    void load_zones();


public:
    toolbar_class(QMainWindow *_monitor, general_class *_global,
                  jobs_class *_jobs, tasks_class *_tasks,
                  servers_class *_servers, groups_class *_groups,
                  log_class *_log, update_class *_update,
                  shared_variables *_shared, QWidget *_settings_widget,
                  properties_class *_properties);
    ~toolbar_class();

    void switch_widget(QString widget_name);
    void hide_properties();

    void set_checked_tasks(bool checked);
    void set_checked_servers(bool checked);
    void set_checked_groups(bool checked);

    inline button *get_tasks_switch() const;
    inline button *get_groups_switch() const;
    inline button *get_servers_switch() const;
};

inline button *toolbar_class::get_tasks_switch() const
{
    return tasks_switch;
}

inline button *toolbar_class::get_groups_switch() const
{
    return groups_switch;
}

inline button *toolbar_class::get_servers_switch() const
{
    return servers_switch;
}

#endif // TOOLBAR_HPP
