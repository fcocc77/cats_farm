// Author: Francisco Contreras
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#ifndef SERVER_STRUCT_H
#define SERVER_STRUCT_H

#include <QString>
#include <QList>

struct inst_struct
{
    int index;
    int status;
    bool reset;
    QString job_task;
};

struct server_struct
{
    QString name;
    QString status;
    QString ip;
    QString system;
    int cpu_used;
    int cpu_iowait;
    int cpu_cores;
    int ram_usage_percent;
    int ram_cache_percent;
    float ram_used;
    float ram_total;
    int cpu_temp;
    QString mac_address;
    int response_time;
    QList<inst_struct *> instances;
    int max_instances;
};

#endif // SERVER_STRUCT_H
