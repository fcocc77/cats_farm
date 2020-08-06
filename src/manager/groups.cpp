#include "manager.hpp"

void manager::group_action(QJsonArray pks)
{

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

            auto group = find_group(name);

            for (QJsonValue s : serverList)
            {
                QString server = s.toString();
                if (not is_struct(group->server, server))
                {

                    QString status = find_server(server)->status;
                    bool _status = true;

                    if (status == "absent")
                    {
                        _status = false;
                    }

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
        // elimina servers del grupo

        for (QJsonValue g : group_machine)
        {
            QJsonArray _group = g.toArray();
            QString name = _group[0].toString();
            QString server = _group[1].toString();

            auto group = find_group(name);

            erase_by_name(group->server, server);
        }

        for (QJsonValue group : group_list)
        {
            erase_by_name(groups, group.toString());
        }
        //------------------------------------------
    }
}

void manager::group_create(QJsonArray pks)
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
        if (is_struct(groups, group_name))
        {
            group_name = group_name_in + "_" + QString::number(pad);
        }
        else
        {
            break;
        }
    }

    group_struct *group = new group_struct;

    group->name = group_name;
    group->status = 0;
    group->totaMachine = totaMachine;
    group->activeMachine = activeMachine;

    //crea vector de server group
    QList<serverFromGroupStruct *> _server;
    for (auto name : server)
    {
        serverFromGroupStruct *sg = new serverFromGroupStruct;
        sg->name = name.toString();
        sg->status = 0;
        _server.push_back(sg);
    }
    group->server = _server;
    //-----------------------------------------------

    groups.push_back(group);
}