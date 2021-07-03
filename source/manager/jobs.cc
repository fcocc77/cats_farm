#include <QTime>

#include "items_util.h"
#include "jobs.h"
#include "manager.h"
#include "threading.h"
#include "util.h"

jobs::jobs(void *__manager)
    : _manager(__manager)
    , items(new QList<job_struct *>)
{
}

void jobs::update()
{
    for (auto job : *items)
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

void jobs::make_job(QJsonObject __job)
{
    // verifica si el nombre esta en la lista, si esta le pone un padding
    QString _job_name = __job["job_name"].toString();
    QString job_name = _job_name;

    for (int i = 0; i < 700; ++i)
    {
        bool contains = false;
        for (auto j : *items)
            if (job_name == j->name)
                contains = true;

        if (contains)
            job_name = _job_name + "_" + QString::number(i);
        else
            break;
    }

    int _task_size = __job["task_size"].toInt();
    int _first_frame = __job["first_frame"].toInt();
    int _last_frame = __job["last_frame"].toInt();

    auto tasks = tasks::make_task(_first_frame, _last_frame, _task_size);

    job_struct *_job = new job_struct;

    _job->name = job_name;
    _job->status = __job["suspended"].toBool() ? "Suspended" : "Queue";
    _job->priority = __job["priority"].toInt();
    _job->server_group = __job["server_group"].toString();
    _job->instances = __job["instances"].toInt();
    _job->comment = __job["comment"].toString();
    _job->software = __job["software"].toString();
    _job->software_data = __job["software_data"].toObject();
    _job->system = __job["system"].toString();
    _job->submit_start = currentDateTime(0);
    _job->submit_finish = "...";
    _job->time_elapsed = 0;
    _job->last_time = 0;
    _job->total_render_time = "...";
    _job->estimated_time = "...";
    _job->time_elapsed_running = 0;
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

    items->push_back(_job);
}

void jobs::job_delete(QString job_name)
{
    erase_by_name(items, job_name);
}

void jobs::job_action(QJsonArray pks)
{
    tasks *_tasks = static_cast<manager *>(_manager)->get_tasks();
    servers *_servers = static_cast<manager *>(_manager)->get_servers();

    for (QJsonValue j : pks)
    {
        QJsonArray _job = j.toArray();
        QString job_name = _job[0].toString();
        QString job_action = _job[1].toString();

        auto job = get_job(job_name);

        if (job_action == "delete")
            _tasks->kill_tasks(job, true);

        if (job_action == "suspend")
        {
            if (not(job->status == "Completed"))
                job->status = "Suspended";

            _tasks->kill_tasks(job, false);
        }

        if (job_action == "unlock")
            job->vetoed_servers.clear();

        if (job_action == "resume")
            if (not(job->status == "Completed"))
                job->status = "Queue";

        if (job_action == "restart")
        {

            _tasks->kill_tasks(job, false);

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
        }

        _servers->reset_all_servers();
    }
}

job_struct *jobs::get_job(QString name) const
{
    for (auto job : *items)
        if (job->name == name)
            return job;

    return items->value(0);
}

QJsonObject jobs::get_options(QString job_name) const
{
    auto job = get_job(job_name);

    QJsonArray _server_group;

    for (QString sg : job->server_group)
        _server_group.push_back(sg);

    QJsonObject options = {
        {"server_group", job->server_group}, {"priority", job->priority},
        {"comment", job->comment},           {"instances", job->instances},
        {"task_size", job->task_size},       {"job_name", job->name},
        {"first_frame", job->first_frame},   {"last_frame", job->last_frame}};

    return options;
}

void jobs::write_options(QString job_name, QJsonObject options)
{
    servers *_servers = static_cast<manager *>(_manager)->get_servers();

    auto job = get_job(job_name);

    job->server_group = options["server_group"].toString();
    job->priority = options["priority"].toInt();
    job->comment = options["comment"].toString();
    job->instances = options["instances"].toInt();

    int _first_frame = options["first_frame"].toInt();
    int _last_frame = options["last_frame"].toInt();
    int _task_size = options["task_size"].toInt();
    QString name = options["job_name"].toString();

    // si el first_frame, last_frame y task_size no se modifican no crea
    // las tares otra vez
    if ((job->first_frame != _first_frame) ||
        (job->last_frame != _last_frame) || (job->task_size != _task_size))
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

        auto tasks =
            tasks::make_task(job->first_frame, job->last_frame, job->task_size);
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
    }

    _servers->reset_all_servers();
}

QString jobs::update_options(QJsonObject pks)
{
    QString action = pks["action"].toString();
    QString job_name = pks["job_name"].toString();
    QJsonObject options = pks["options"].toObject();

    if (action == "write")
        write_options(job_name, options);

    else if (action == "read")
        return jots(get_options(job_name));

    return "";
}

QString jobs::job_log_action(QString server_name)
{
    servers *_servers = static_cast<manager *>(_manager)->get_servers();

    auto server = _servers->get_server(server_name);
    QString result = server->log;

    return result;
}
