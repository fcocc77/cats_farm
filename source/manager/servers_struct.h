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
    QString host;
    QString system;
    int cpu;
    int cpu_iowait;
    int cpu_cores;
    int ram;
    int ram_cached;
    float ram_used;
    float ram_total;
    int temp;
    QString mac;
    int response_time;
    QList<inst_struct *> instances;
    int max_instances;
    QString sshUser;
    QString sshPass;
    QString vmSoftware;
    QString log;
};

#endif // SERVER_STRUCT_H
