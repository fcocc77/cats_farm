#ifndef JOBS_HPP
#define JOBS_HPP

#include "includes.hpp"
#include "log.hpp"
#include "servers.hpp"
#include "options.hpp"
#include "groups.hpp"
#include "properties.hpp"

class jobs_class : public QTreeWidget
{
private:
    QTreeWidget *jobs;
    shared_variables *shared;
    QMainWindow *monitor;
    log_class *log;
    servers_class *servers;
    options_class *options;
    groups_class *groups;
    properties_class *properties;
    void connections();
    virtual void mousePressEvent(QMouseEvent *event);

    // Jobs Funcions
    void setup_ui();
    void popup();
    void show_log();
    void modify();
    void options_ok();
    void delete_start(QString action);
    void to_action(QString action);

    void message(
        void (jobs_class::*funtion)(QString),
        QString action,
        QString ask,
        QString tile,
        jobs_class *_class);

    void item_delete();
    //-----------------------------
    QTreeWidgetItem *first_job_item;

    // log
    QString log_server;
    // --------------------------

public:
    jobs_class(
        shared_variables *_shared,
        QMainWindow *_monitor,
        log_class *_log,
        servers_class *_servers,
        options_class *_options,
        groups_class *_groups,
        properties_class *_properties);
    ~jobs_class();

    // Job Acciones
    QAction *delete_action;
    QAction *job_suspend_action;
    QAction *job_resume_action;
    QAction *job_restart_action;
    QAction *job_unlock_servers_action;
    QAction *job_log_action;
    QAction *job_modify_action;
    //------------------------------------------------
};

#endif //JOBS_HPP