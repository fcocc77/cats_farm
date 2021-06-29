#include <manager.h>

void manager::update_jobs()
{
    for (auto job : jobs)
    {
        int waiting = job->tasks -
                      (job->suspended_task + job->progres + job->active_task);
        job->waiting_task = waiting;

        if (job->failed_task)
        {
            threading([=]() {
                job->status = "Failed";
                sleep(7);
                job->status = "Queue";
            });

            job->failed_task = 0;
        }
        else
        {
            if ((job->status == "Suspended") or (job->status == "Completed") or
                (job->status == "Failed") or (job->status == "Concatenate"))
                ;
            else if (job->active_task)
                job->status = "Rendering...";
            else
                job->status = "Queue";
        }

        if (job->active_task)
        {
            // tiempo actual de inicio de los dias
            int current_time = QTime::currentTime().msecsSinceStartOfDay();

            int add = 0;
            if (job->time_elapsed_running)
                // tiempo actual menos el ultimo tiempo registrado
                add = current_time - job->last_time;

            job->time_elapsed += add;

            // evita que el tiempo se dispare cuando inicia la cuenta,
            // esto pasa cuando se resta el 'current_time' por 'job->last_time'
            // que al inicio es cero.
            if (job->time_elapsed > 50000000)
                job->time_elapsed = 0;

            int estimated_time;
            if (job->progres)
            {
                // calcula el tiempo estimado
                int remaining = job->tasks - job->progres;
                estimated_time = (job->time_elapsed * remaining) / job->progres;
                if (estimated_time > job->estimated_time_ms)
                    if (job->estimated_time_ms)
                        estimated_time = job->estimated_time_ms;
            }
            else
                estimated_time = 0;

            job->estimated_time_ms = estimated_time;

            // convierte los milisegundo en segundos y luego al formato de
            // tiempo en string, para el tiempo estimado y restante.
            if (estimated_time)
                job->estimated_time = secToTime(estimated_time / 1000);
            else
                job->estimated_time = "...";
            job->total_render_time = secToTime(job->time_elapsed / 1000);

            job->last_time = current_time;
            job->time_elapsed_running = true;
        }
        else
        {
            job->time_elapsed_running = false;
            job->estimated_time = "...";
        }
    }
}

QString manager::make_job(QJsonArray recv)
{
    QString _job_name = recv[0].toString();

    QStringList _server_group;
    if (recv[2].toString() != "None")
        _server_group.push_back(recv[2].toString());

    int _first_frame = recv[3].toInt();
    int _last_frame = recv[4].toInt();
    int _task_size = recv[5].toInt();
    int _priority = recv[6].toInt();
    bool _suspend = recv[7].toBool();
    QString _comment = recv[8].toString();
    QString _software = recv[9].toString();
    QString _project = recv[10].toString();
    QString _misc = recv[11].toString();
    QString _system = recv[12].toString();
    int _instances = recv[13].toInt();
    QString _render = recv[14].toString();

    print(recv);

    QString status, submit_start;

    if (_suspend)
        status = "Suspended";
    else
        status = "Queue";

    submit_start = currentDateTime(0);

    // checkea si el nombre esta en la lista, si esta le pone un padding
    QString job_name = _job_name;

    for (int i = 0; i < 100; ++i)
    {
        bool inside = false;
        for (auto j : jobs)
            if (job_name == j->name)
                inside = true;

        if (inside)
            job_name = _job_name + "_" + QString::number(i);
        else
            break;
    }

    auto tasks = make_task(_first_frame, _last_frame, _task_size);

    job_struct *_job = new job_struct;

    _job->name = job_name;
    _job->status = status;
    _job->priority = _priority;
    _job->server_group = _server_group;
    _job->instances = _instances;
    _job->comment = _comment;
    _job->submit_start = submit_start;
    _job->submit_finish = "...";
    _job->time_elapsed = 0;
    _job->last_time = 0;
    _job->total_render_time = "...";
    _job->estimated_time = "...";
    _job->time_elapsed_running = 0;
    _job->software = _software;
    _job->project = _project;
    _job->system = _system;
    _job->misc = _misc;
    _job->render = _render;
    _job->progres = 0;
    _job->errors = 0;
    _job->waiting_task = tasks.size();
    _job->tasks = tasks.size();
    _job->suspended_task = 0;
    _job->failed_task = 0;
    _job->active_task = 0;
    _job->task_size = _task_size;
    _job->task = tasks;
    _job->first_frame = _first_frame;
    _job->last_frame = _last_frame;

    jobs.push_back(_job);

    return "";
}

void manager::job_delete(QString job_name)
{
    erase_by_name(jobs, job_name);
}

void manager::job_action(QJsonArray pks)
{
    for (QJsonValue j : pks)
    {
        QJsonArray _job = j.toArray();
        QString job_name = _job[0].toString();
        QString job_action = _job[1].toString();

        auto job = get_job(job_name);

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
            job->errors = 0;
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

job_struct *manager::get_job(QString name)
{
    for (auto job : jobs)
        if (job->name == name)
            return job;
    return jobs[0];
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

        auto job = get_job(job_name);

        if (action == "write")
        {
            job->server_group.clear();
            for (QJsonValue sg : options[1].toArray())
                job->server_group.push_back(sg.toString());

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

            // si el first_frame, last_frame y task_size no se modifican no crea
            // las tares otra vez
            if ((job->first_frame != _first_frame) or
                (job->last_frame != _last_frame) or
                (job->task_size != _task_size))
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
                        for (int i = task->first_frame; i <= task->last_frame;
                             ++i)
                            suspended.push_back(i);

                    if ((task->status == "finished"))
                        for (int i = task->first_frame; i <= task->last_frame;
                             ++i)
                            finished.push_back(i);
                }
                //----------------------------------------------------

                auto tasks = make_task(job->first_frame, job->last_frame,
                                       job->task_size);
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
            QJsonArray _server_group;
            for (QString sg : job->server_group)
                _server_group.push_back(sg);

            return jats({"", _server_group, job->priority, job->comment,
                         job->instances, job->task_size, job->name,
                         job->first_frame, job->last_frame});
        }
    }

    return "";
}

QString manager::job_log_action(QString server_name)
{
    auto server = get_server(server_name);
    QString result = server->log;

    return result;
}
