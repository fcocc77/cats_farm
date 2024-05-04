// Author: Francisco Contreras
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#ifndef TASKS_HPP
#define TASKS_HPP

#include "jobs_widget.h"
#include "shared_variables.h"
#include "title_bar.h"

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

    shared_variables *shared;
    jobs_class *jobs;

    QTreeWidget *tree;
    QWidget *tasks_widget;
    QVBoxLayout *layout;
    title_bar *_title_bar;

public:
    tasks_class(QMainWindow *_monitor, shared_variables *_shared,
                jobs_class *_jobs);
    ~tasks_class();

    // Task Action
    QAction *pause_action;
    QAction *restart_action;
    QAction *render_server_action;

    inline QTreeWidget *get_tree() const;
    inline QWidget *get_widget() const;
};

inline QWidget *tasks_class::get_widget() const
{
    return tasks_widget;
}

inline QTreeWidget *tasks_class::get_tree() const
{
    return tree;
}

#endif // TASKS_HPP
