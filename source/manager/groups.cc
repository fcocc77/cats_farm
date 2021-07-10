#include "groups.h"
#include "items_util.h"
#include "manager.h"
#include "jobs.h"
#include "servers.h"

groups::groups(void *__manager)
    : _manager(__manager)
    , items(new QList<group_struct *>)
{
}

void groups::update()
{
    manager *__manager = static_cast<manager *>(_manager);

    jobs *_jobs = __manager->get_jobs();
    servers *_servers = __manager->get_servers();

    // Obtiene todos los grupos que estan activos
    QStringList groups_used;
    for (auto job : *_jobs->get_items())
        if (job->active_task)
            for (QString sg : job->server_group)
                groups_used.push_back(sg);

    QJsonArray grouplist;

    for (auto group : *items)
    {
        grouplist.push_back(group->name);
        int totaMachine = 0, activeMachine = 0;

        for (auto _server : group->server)
        {

            if (contains(_servers->get_items(), _server->name))
            {
                auto server = _servers->get_server(_server->name);

                totaMachine++;
                if (server->status == "absent")
                    _server->status = false;
                else
                {
                    _server->status = true;
                    activeMachine++;
                }
            }
            else
            {
                erase_by_name(&group->server, _server->name);
            }
        }
        bool server_status;
        if (groups_used.contains(group->name))
            server_status = true;
        else
            server_status = false;

        group->status = server_status;
        group->totaMachine = totaMachine;
        group->activeMachine = activeMachine;
    }

    __manager->get_settings()->insert("groups", grouplist);
}

void groups::group_action(QJsonArray pks)
{
    servers *_servers = static_cast<manager *>(_manager)->get_servers();

    QJsonArray group_list = pks[0].toArray();
    QJsonArray group_machine = pks[1].toArray();
    QString group_action = pks[2].toString();

    if (group_action == "addMachine")
    {
        for (QJsonValue g : group_machine)
        {
            QJsonArray _group = g.toArray();
            QString name = _group[0].toString();
            QJsonArray serverList = _group[1].toArray();

            auto group = get_group(name);

            for (QJsonValue s : serverList)
            {
                QString server = s.toString();
                if (not contains(&group->server, server))
                {

                    QString status = _servers->get_server(server)->status;
                    bool _status = true;

                    if (status == "absent")
                        _status = false;

                    serverFromGroupStruct *sg = new serverFromGroupStruct;
                    sg->name = server;
                    sg->status = _status;

                    group->server.push_back(sg);
                }
            }
        }
    }

    if (group_action == "delete")
    {
        // Elimina los 'servers' del grupo
        for (QJsonValue g : group_machine)
        {
            QJsonArray _group = g.toArray();
            QString name = _group[0].toString();
            QString server = _group[1].toString();

            auto group = get_group(name);

            erase_by_name(&group->server, server);
        }

        for (QJsonValue group : group_list)
            erase_by_name(items, group.toString());
    }
}

group_struct *groups::get_group(QString name)
{
    for (auto group : *items)
        if (group->name == name)
            return group;

    return items->value(0);
}

void groups::group_create(QJsonArray pks)
{

    QString group_name_in = pks[0].toString();
    int totaMachine = pks[1].toInt();
    int activeMachine = pks[2].toInt();
    QJsonArray server = pks[3].toArray();

    QString group_name = group_name_in;

    int pad = 0;
    while (1)
    {
        pad += 1;
        if (contains(items, group_name))
            group_name = group_name_in + "_" + QString::number(pad);
        else
            break;
    }

    group_struct *group = new group_struct;

    group->name = group_name;
    group->status = 0;
    group->totaMachine = totaMachine;
    group->activeMachine = activeMachine;

    // crea vector de server group
    QList<serverFromGroupStruct *> _server;
    for (auto name : server)
    {
        serverFromGroupStruct *sg = new serverFromGroupStruct;
        sg->name = name.toString();
        sg->status = 0;
        _server.push_back(sg);
    }
    group->server = _server;

    items->push_back(group);
}
