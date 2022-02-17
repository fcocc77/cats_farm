// Author: Francisco Jose Contreras Cuevas
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#ifndef JOBS_STRUCT_H
#define JOBS_STRUCT_H

#include <QString>
#include <QJsonObject>
#include <QStringList>

#include "tasks_struct.h"

struct job_struct
{
    QString name;
    QString status;
    int priority;
    QString server_group;
    int instances;
    QString comment;
    QString submit_start;
    QString submit_finish;
    int errors;
    int time_elapsed;
    int last_time;
    int estimated_time_ms;
    bool time_elapsed_running;
    QString total_render_time;
    QString estimated_time;
    QString software;
    QJsonObject software_data;
    QString system;
    QStringList vetoed_servers;
    int progres;
    int waiting_task;
    int tasks;
    int paused_task;
    int failed_task;
    int active_task;
    int task_size;
    int first_frame;
    int last_frame;
    QList<task_struct *> task;
};

struct makejob_struct
{
    QString job_name;
    QString server;
    QString server_group;
    int first_frame;
    int last_frame;
    int task_size;
    int priority;
    bool paused;
    QString comment;
    QString software;
    QString project;
    QString extra;
    QString system;
    int instances;
    QString render;
};

#endif // JOBS_STRUCT_H
