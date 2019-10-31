#ifndef TASKS_HPP
#define TASKS_HPP

#include "includes.hpp"

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
    QString manager_host;

public:
    tasks_class(
        Ui::MainWindow *_ui,
        QMainWindow *_monitor);
    ~tasks_class();

    // Task Action
    QAction *suspend_action;
    QAction *restart_action;
    QAction *render_server_action;
    //------------------------------------------------
};

#endif //TASKS_HPP