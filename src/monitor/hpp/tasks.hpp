#ifndef TASKS_HPP
#define TASKS_HPP

#include "includes.hpp"
#include "shared_variables.hpp"

class tasks_class : public QObject
{
private:
    Ui::MainWindow *ui;
    QMainWindow *monitor;

    // Task Funtions
    void properties();
    void popup();
    void restart();
    void render_server();
    void message(QString action, QString ask, QString tile);
    void to_action(QString action);
    void connections();
    //-----------------
    shared_variables *shared;

public:
    tasks_class(
        Ui::MainWindow *_ui,
        QMainWindow *_monitor,
        shared_variables *_shared);
    ~tasks_class();

    // Task Action
    QAction *suspend_action;
    QAction *restart_action;
    QAction *render_server_action;
    //------------------------------------------------
};

#endif //TASKS_HPP