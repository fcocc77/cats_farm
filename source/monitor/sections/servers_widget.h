#ifndef SERVERS_HPP
#define SERVERS_HPP

#include <QTreeWidget>
#include <QVBoxLayout>

#include "log.h"
#include "shared_variables.h"
#include "title_bar.h"

class servers_class : public QWidget
{
    Q_OBJECT
private:
    QWidget *_monitor;
    shared_variables *shared;
    log_class *log;

    QVBoxLayout *layout;
    QTreeWidget *tree;
    title_bar *_title_bar;

    // Server Funcions
    void setup_ui();
    void connections();
    void server_popup();
    void cpu_limit(int limit);
    void to_log();
    void message(QString (servers_class::*funtion)(QString, QString),
                 QString action, QString ask, QString tile, QString info,
                 servers_class *_class);
    QString to_action(QString action, QString info);
    QString send_to_vserver(QString action, QString info);

    QTreeWidgetItem *firstServerItem;

public:
    servers_class(QWidget *_monitor, shared_variables *_shared,
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

    // Server Action
    QAction *server_inactive_action;
    QAction *server_reactive_action;
    QAction *server_max_instances_action;
    QAction *server_free_ram_action;
    QAction *server_turn_on_action;
    QAction *server_turn_off_action;
    QAction *server_show_log;
    QAction *delete_action;

    inline QTreeWidget *get_tree() const;

public slots:
    void server_max_instances(int);
};

inline QTreeWidget *servers_class::get_tree() const
{
    return tree;
}

#endif // SERVERS_HPP
