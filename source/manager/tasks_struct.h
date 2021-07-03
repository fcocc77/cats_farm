#ifndef TASKS_STRUCT_H
#define TASKS_STRUCT_H

#include <QString>

struct task_struct
{
    QString name;
    QString status;
    int first_frame;
    int last_frame;
    QString server;
    QString time;
};


#endif // TASKS_STRUCT_H
