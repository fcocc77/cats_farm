#include <manager.h>

vector<task_struct *> manager::make_task(int first_frame, int last_frame, int task_size)
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

        range ={ f, l };
        tasks_range.push_back(range);

        if (l == last_frame)
            break;
    }

    // Creacion de tareas
    vector<task_struct *> tasks;

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

        task_struct *_tasks = new task_struct;

        _tasks->name = task_name;
        _tasks->status = "waiting";
        _tasks->first_frame = i[0];
        _tasks->last_frame = i[1];
        _tasks->server = "...";
        _tasks->time = "...";

        tasks.push_back(_tasks);
    }

    return tasks;
}

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

task_struct *manager::get_task(vector<task_struct *> tasks, QString name)
{
    for (auto task : tasks)
        if (task->name == name)
            return task;
    return tasks[0];
}

void manager::task_action(QJsonArray pks)
{
    for (QJsonValue t : pks)
    {
        QJsonArray _task = t.toArray();
        QString job_name = _task[0].toString();
        QString task_name = _task[1].toString();
        QString task_action = _task[2].toString();

        auto job = get_job(job_name);
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