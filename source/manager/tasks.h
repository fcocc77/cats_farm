#ifndef TASKS_H
#define TASKS_H

#include <QList>
#include <QString>
#include <QJsonArray>

#include "tasks_struct.h"

class tasks
{
private:
    void *_manager;
    QJsonObject *settings;

    task_struct *get_task(QList<task_struct *> tasks, QString name);

public:
    tasks(void *_manager);

    void kill_tasks(void *job, bool _delete);
    static QList<task_struct *> make_task(int first_frame, int last_frame,
                                   int task_size);

    void task_action(QJsonArray pks);
};

#endif // TASKS_H
