#ifndef TASKS_HPP
#define TASKS_HPP

#include "includes.h"
#include "shared_variables.h"
#include "jobs.h"

class tasks_class : public QObject
{
private:
    QMainWindow *monitor;

    // Task Funtions
    void setup_ui();
    void popup();
    void restart();
    void render_server();
    void message(QString action, QString ask, QString tile);
    void to_action(QString action);
    void connections();
    //-----------------
    shared_variables *shared;
    jobs_class *jobs;

public:
    tasks_class(
        QMainWindow *_monitor,
        shared_variables *_shared,
        jobs_class *_jobs);
    ~tasks_class();

    // Task Action
    QAction *suspend_action;
    QAction *restart_action;
    QAction *render_server_action;
    //-----------------------------

    QTreeWidget *tree;

};

#endif //TASKS_HPP