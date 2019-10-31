#ifndef JOBS_HPP
#define JOBS_HPP

#include "includes.hpp"
#include "log.hpp"

class jobs_class : public QObject
{
private:
    Ui::MainWindow *ui;
    QTreeWidget *jobs;
    shared_variables *shared;
    QMainWindow *monitor;
    log_class *log;
    void properties();
    void connections();
    // virtual void mousePressEvent(QMouseEvent *event);

    // Jobs Funcions
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
    QString manager_host;

    // log
    QString log_server;
    // --------------------------

public:
    jobs_class(
        Ui::MainWindow *_ui,
        shared_variables *_shared,
        QMainWindow *_monitor,
        log_class *_log);
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