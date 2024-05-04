// Author: Francisco Contreras
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#ifndef GROUPS_STRUCT_H
#define GROUPS_STRUCT_H

#include <QString>
#include <QList>

struct serverFromGroupStruct
{
    QString name;
    bool status;
};

struct group_struct
{
    QString name;
    bool status;
    int totaMachine;
    int activeMachine;
    QList<serverFromGroupStruct *> server;
};

#endif // GROUPS_STRUCT_H
