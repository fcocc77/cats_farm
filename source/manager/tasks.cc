#include "tasks.h"
#include "jobs.h"
#include "manager.h"
#include "items_util.h"
#include "tcp.h"

tasks::tasks(void *__manager)
    : _manager(__manager)
{
    manager *___manager = static_cast<manager *>(_manager);
    settings = ___manager->get_settings();
    server_port = settings->value("server").toObject()["port"].toInt();
}

QList<task_struct *> tasks::make_task(int first_frame, int last_frame,
                                         int task_size)
{
    // Crea una lista de tareas con los 'frames' de inicio y final
    QList<int> range;
    QList<QList<int>> tasks_range;

    int t = task_size - 1;
    int f = first_frame - task_size;
    int s, l;

    while (1)
    {
        f = f + t + 1;
        for (int i = 0; i < 10000; i++)
        {
            s = f + t;
            l = f + i;
            if (l == s or l == last_frame)
                break;
        }

        range = {f, l};
        tasks_range.push_back(range);

        if (l == last_frame)
            break;
    }

    // Creacion de tareas
    QList<task_struct *> _tasks;

    int num = 0;
    QString task_name;
    for (auto i : tasks_range)
    {
        num++;

        if (num < 10)
            task_name = "Task_00" + QString::number(num);
        else if (num > 99)
            task_name = "Task_" + QString::number(num);
        else
            task_name = "Task_0" + QString::number(num);

        task_struct *_task = new task_struct;

        _task->name = task_name;
        _task->status = "waiting";
        _task->first_frame = i[0];
        _task->last_frame = i[1];
        _task->server = "...";
        _task->time = "...";

        _tasks.push_back(_task);
    }

    return _tasks;
}

void tasks::kill_tasks(void *_job, bool _delete)
{
    job_struct *job = static_cast<job_struct*>(_job);
    jobs *_jobs = static_cast<manager *>(_manager)->get_jobs();
    servers *_servers = static_cast<manager *>(_manager)->get_servers();

    // desactica servers que estaban activos por este job
    QStringList active_server;
    for (auto task : job->task)
    {
        if (not(task->server == "..."))
        {
            if (task->status == "active")
            {
                QString name = task->server.split(":")[0];
                if (not active_server.contains(name))
                    active_server.push_back(name);
            }
        }
    }

    if (_delete)
        erase_by_name(_jobs->get_items(), job->name);

    QJsonArray kill_ins;
    for (int i = 0; i < 15; i++)
        kill_ins.push_back(i);

    for (auto server : *_servers->get_items())
        if (active_server.contains(server->name))
            tcpClient(server->host, server_port, jats({3, kill_ins}));
}

task_struct *tasks::get_task(QList<task_struct *> tasks, QString name)
{
    for (auto task : tasks)
        if (task->name == name)
            return task;

    return tasks[0];
}

void tasks::task_action(QJsonArray pks)
{
    jobs *_jobs = static_cast<manager *>(_manager)->get_jobs();

    for (QJsonValue t : pks)
    {
        QJsonArray _task = t.toArray();
        QString job_name = _task[0].toString();
        QString task_name = _task[1].toString();
        QString task_action = _task[2].toString();

        auto job = _jobs->get_job(job_name);
        auto task = get_task(job->task, task_name);

        if (task_action == "suspend")
        {
            if (task->status == "waiting")
            {
                job->suspended_task += 1;
                task->status = "suspended";
                job->status = "Queue ";
            }
        }

        if (task_action == "resume")
        {
            if (not(task->status == "active"))
            {
                if (task->status == "suspended")
                    job->suspended_task -= 1;

                if (task->status == "finished")
                    job->progres -= 1;

                if (task->status == "failed")
                    job->failed_task -= 1;

                task->status = "waiting";
                task->server = "...";
                task->time = "...";
                job->status = "Queue";
            }
        }
    }
}
