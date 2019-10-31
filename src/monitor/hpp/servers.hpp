#ifndef SERVERS_HPP
#define SERVERS_HPP

#include "includes.hpp"
#include "log.hpp"

class servers_class : public QObject
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    QMainWindow *monitor;
    shared_variables *shared;
    log_class *log;

    // Server Funcions
    void properties();
    void connections();
    void server_popup();
    void cpu_limit(int limit);
    void to_log();
    void ssh_client();
    void vnc_client();
    void message(
        QString (servers_class::*funtion)(QString, QString),
        QString action,
        QString ask,
        QString tile,
        QString info,
        servers_class *_class);
    void message_action();
    QString to_action(QString action, QString info);
    void send_to_vserver(QString action, QString info);
    //----------------

    QTreeWidgetItem *firstServerItem;

    const QString managerHost = fread(path + "/etc/manager_host");

public:
    servers_class(
        Ui::MainWindow *_ui,
        QMainWindow *_monitor,
        shared_variables *_shared,
        log_class *_log);
    ~servers_class();

    // display job list
    QAction *show_all_action;
    QAction *hide_all_action;
    QAction *display_windows_action;
    QAction *display_linux_action;
    QAction *display_mac_action;
    QAction *display_on_action;
    QAction *display_off_action;
    //------------------------------------------------

    // Server Action
    QAction *server_inactive_action;
    QAction *server_reactive_action;
    QAction *server_max_instances_action;
    QAction *server_free_ram_action;
    QAction *server_ssh_action;
    QAction *server_show_log;
    QAction *server_vnc_action;
    QAction *delete_action;
    //------------------------------------------------

public slots:
    void server_max_instances(int);
};

#endif //SERVERS_HPP