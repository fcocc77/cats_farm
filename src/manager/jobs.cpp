#include "manager.hpp"

void manager::job_delete(QString job_name) {
    erase_by_name(jobs, job_name);
}

void manager::job_action(QJsonArray pks)
{
    for (QJsonValue j : pks)
    {
        QJsonArray _job = j.toArray();
        QString job_name = _job[0].toString();
        QString job_action = _job[1].toString();

        auto job = find_job(job_name);

        if (job_action == "delete")
            kill_tasks(job, true);

        if (job_action == "suspend")
        {
            if (not(job->status == "Completed"))
                job->status = "Suspended";

            kill_tasks(job, false);
        }

        if (job_action == "unlock")
            job->vetoed_servers.clear();

        if (job_action == "resume")
            if (not(job->status == "Completed"))
                job->status = "Queue";

        if (job_action == "restart")
        {

            kill_tasks(job, false);

            job->status = "Queue";
            job->progres = 0;
            job->suspended_task = 0;
            job->active_task = 0;
            job->time_elapsed = 0;
            job->last_time = 0;
            job->submit_finish = "...";
            job->total_render_time = "...";
            job->vetoed_servers.clear();

            // reset tasks
            for (auto task : job->task)
            {
                task->status = "waiting";
                task->server = "...";
                task->time = "...";
            }
            //-------------------------------------
        }

        reset_all_servers();
    }
}

QString manager::job_options(QJsonArray pks)
{
    int num = 0;
    QString _name;
    for (QJsonValue j : pks)
    {
        QJsonArray _job = j.toArray();
        QString job_name = _job[0].toString();
        QJsonArray options = _job[1].toArray();
        QString action = _job[2].toString();

        auto job = find_job(job_name);

        if (action == "write")
        {
            job->server.clear();
            for (QJsonValue s : options[0].toArray())
                job->server.push_back(s.toString());
            //------------------------
            job->server_group.clear();
            for (QJsonValue sg : options[1].toArray())
                job->server_group.push_back(sg.toString());
            //------------------------
            job->priority = options[2].toInt();
            job->comment = options[3].toString();
            job->instances = options[4].toInt();
            int _first_frame = options[5].toInt();
            int _last_frame = options[6].toInt();
            int _task_size = options[7].toInt();

            // el nombre se repite se pone un numero al final del nombre
            QString name = options[8].toString();
            if (not num)
                job->name = name;
            else
                job->name = name + "_" + QString::number(num);
            num++;
            //------------------------------------------------

            // si el first_frame, last_frame y task_size no se modifican no crea las tares otra vez
            if ((job->first_frame != _first_frame) or (job->last_frame != _last_frame) or (job->task_size != _task_size))
            {

                job->first_frame = _first_frame;
                job->last_frame = _last_frame;
                job->task_size = _task_size;

                // obtiene los frames segun el estado
                QList<int> finished;
                QList<int> suspended;
                for (auto task : job->task)
                {
                    if ((task->status == "suspended"))
                        for (int i = task->first_frame; i <= task->last_frame; ++i)
                            suspended.push_back(i);

                    if ((task->status == "finished"))
                        for (int i = task->first_frame; i <= task->last_frame; ++i)
                            finished.push_back(i);
                }
                //----------------------------------------------------

                auto tasks = make_task(job->first_frame, job->last_frame, job->task_size);
                job->task = tasks;
                job->waiting_task = tasks.size();
                job->tasks = tasks.size();

                int progres = 0;
                for (auto task : job->task)
                {
                    int task_size = task->last_frame - task->first_frame + 1;

                    int _finished = 0;
                    int _suspended = 0;

                    for (int i = task->first_frame; i <= task->last_frame; ++i)
                    {
                        for (int f : finished)
                            if (i == f)
                                _finished++;
                        for (int s : suspended)
                            if (i == s)
                                _suspended++;
                    }

                    if (task_size == _finished)
                    {
                        task->status = "finished";
                        progres++;
                    }
                    if (task_size == _suspended)
                        task->status = "suspended";
                }
                job->progres = progres;
            } //----------------------------------------

            reset_all_servers();
        }

        if (action == "read")
        {
            QJsonArray _server;
            for (QString s : job->server)
                _server.push_back(s);
            //----------------------------------------------------
            QJsonArray _server_group;
            for (QString sg : job->server_group)
                _server_group.push_back(sg);
            //----------------------------------------------------
            return jats({ _server, _server_group, job->priority, job->comment, job->instances,
                job->task_size, job->name, job->first_frame, job->last_frame });
        }
    }

    return "";
}

QString manager::job_log_action(QString server_name)
{
    auto server = find_server(server_name);
    QString result = server->log;

    return result;
}
