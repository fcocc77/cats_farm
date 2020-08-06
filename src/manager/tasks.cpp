#include "manager.hpp"

void manager::kill_tasks(job_struct *job, bool _delete)
{
    //desactica servers que estaban activos por este job
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
        erase_by_name(jobs, job->name);

    QJsonArray kill_ins;
    for (int i = 0; i < 15; i++)
        kill_ins.push_back(i);

    for (auto server : servers)
        if (active_server.contains(server->name))
            tcpClient(server->host, server_port, jats({ 3, kill_ins }));
}

void manager::task_action(QJsonArray pks)
{
    for (QJsonValue t : pks)
    {
        QJsonArray _task = t.toArray();
        QString job_name = _task[0].toString();
        QString task_name = _task[1].toString();
        QString task_action = _task[2].toString();

        auto job = find_job(job_name);
        auto task = find_task(job->task, task_name);

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
                {
                    job->suspended_task -= 1;
                }

                if (task->status == "finished")
                {
                    job->progres -= 1;
                }

                if (task->status == "failed")
                {
                    job->failed_task -= 1;
                }

                task->status = "waiting";
                task->server = "...";
                task->time = "...";
                job->status = "Queue";
            }
        }
    }
}