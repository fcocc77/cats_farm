// Author: Francisco Contreras
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <QJsonObject>
#include <QString>

#include "../global/global.h"
#include "groups.h"
#include "jobs.h"
#include "renderer.h"
#include "servers.h"
#include "tasks.h"

class manager : public QObject
{
private:
    jobs *_jobs;
    servers *_servers;
    groups *_groups;
    tasks *_tasks;
    renderer *_renderer;

    QMutex *mutex;

    QJsonObject *settings;

    QString pivot_to_server(QJsonArray recv);
    QString send_to_monitor_thread();
    QString send_to_logger();
    void reactive_all();
    QString recieve_monitor_thread(QJsonArray recv);
    QString settings_action(QJsonArray pks);
    QString server_tcp(QString recv);

    void container_save();
    void update_all();

public:
    manager();

    inline jobs *get_jobs() const;
    inline servers *get_servers() const;
    inline groups *get_groups() const;
    inline tasks *get_tasks() const;
    inline QJsonObject *get_settings() const;
    inline renderer *get_renderer() const;
};

inline renderer *manager::get_renderer() const
{
    return _renderer;
}

inline QJsonObject *manager::get_settings() const
{
    return settings;
}

inline jobs *manager::get_jobs() const
{
    return _jobs;
}

inline servers *manager::get_servers() const
{
    return _servers;
}

inline groups *manager::get_groups() const
{
    return _groups;
}

inline tasks *manager::get_tasks() const
{
    return _tasks;
}

#endif // MANAGER_HPP
