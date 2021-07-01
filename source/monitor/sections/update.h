#ifndef UPDATE_HPP
#define UPDATE_HPP

#include "tcp.h"
#include "groups_widget.h"
#include "jobs_widget.h"
#include "servers_widget.h"
#include "settings.h"
#include "progress_bar.h"

class update_class : public QObject
{
private:
    // variables
    shared_variables *shared;
    groups_class *groups;
    jobs_class *jobs;
    servers_class *servers;
    QTreeWidget *tasks;
    QStringList delete_list;
    tcp_client_widget<update_class> *manager;
    settings_class *settings;
    bool update_started = false;

    // Tasks
    bool task_first_add;
    QThread *get_task_thread;

    // funciones
    QString manager_recieve_update(QString recv, QJsonObject extra);
    void update_jobs(QJsonObject recv);
    void update_servers(QJsonObject recv);
    void update_groups(QJsonObject recv);
    void update_task();

public:
    update_class(shared_variables *_shared, groups_class *_groups,
                 jobs_class *_jobs, servers_class *_servers,
                 QTreeWidget *_tasks_tree, settings_class *_settings);

    void update(QString host);
    void get_task();
};

#endif // UPDATE_HPP
