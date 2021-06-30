#ifndef JOBS_HPP
#define JOBS_HPP

#include <QTreeWidget>

#include "options.h"
#include "groups.h"
#include "log.h"
#include "properties.h"
#include "servers.h"
#include "submit.h"

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
    submit *_submit;
    void connections();

    // Jobs Funcions
    void setup_ui();
    void popup();
    void show_log();
    void modify();
    void options_ok();
    void delete_start(QString action);
    void to_action(QString action);

    void message(void (jobs_class::*funtion)(QString), QString action,
                 QString ask, QString tile, jobs_class *_class);

    void item_delete();

    QTreeWidgetItem *first_job_item;

    QString log_server;

    virtual void mousePressEvent(QMouseEvent *event) override;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

public:
    jobs_class(shared_variables *_shared, QMainWindow *_monitor,
               log_class *_log, servers_class *_servers,
               options_class *_options, groups_class *_groups,
               properties_class *_properties, submit *_submit);
    ~jobs_class();

    // Job Acciones
    QAction *delete_action;
    QAction *job_suspend_action;
    QAction *job_resume_action;
    QAction *job_restart_action;
    QAction *job_unlock_servers_action;
    QAction *job_log_action;
    QAction *job_modify_action;
};

#endif // JOBS_HPP
