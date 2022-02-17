// Author: Francisco Jose Contreras Cuevas
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#include "manager.h"
#include "struct_transform.h"
#include "tcp.h"
#include "threading.h"

manager::manager()
    : mutex(new QMutex)
    , settings(new QJsonObject)
{
    _servers = new servers(this);
    _tasks = new tasks(this);
    _jobs = new jobs(this);
    _groups = new groups(this);

    _renderer = new renderer(this, mutex, _jobs, _servers, _groups);

    QJsonObject info = jread(VINARENDER_CONF_PATH + "/info.json");
    if (not info.empty())
        json_to_struct(info, _jobs, _servers, _groups);

    reactive_all();
    // Recive la informacion del suministrador y crea un jobs con sus tareas

    *settings = jread(VINARENDER_CONF_PATH + "/settings.json");

    tcpServer(MANAGER_PORT, &manager::server_tcp, this);
    threading(&manager::update_all, this);
}

QString manager::server_tcp(QString _recv)
{
    QJsonArray recv = jafs(_recv);
    int input = recv[0].toInt();
    QJsonArray pks = recv[1].toArray();

    QString send;

    mutex->lock();

    if (input == 1)
        send = _servers->update_server_thread(recv[1].toObject());
    else if (input == 2)
        send = send_to_monitor_thread();
    else if (input == 3)
        send = recieve_monitor_thread(pks);
    else if (input == 4)
        _jobs->make_job(recv[1].toObject());
    else if (input == 5)
        send = pivot_to_server(pks);
    else if (input == 6)
        send = send_to_logger();

    mutex->unlock();

    return send;
}

QString manager::send_to_logger()
{
    QJsonObject __jobs;
    for (auto job : *_jobs->get_items())
    {
        QJsonObject j;

        j["name"] = job->name;
        j["status"] = job->status;
        j["comment"] = job->comment;
        j["submit_start"] = job->submit_start;
        j["submit_finish"] = job->submit_finish;
        j["total_render_time"] = job->total_render_time;
        j["estimated_time"] = job->estimated_time;
        j["time_elapsed_running"] = job->time_elapsed_running;
        j["progres"] = job->progres;
        j["waiting_task"] = job->waiting_task;
        j["tasks"] = job->tasks;
        j["paused_task"] = job->paused_task;
        j["failed_task"] = job->failed_task;
        j["active_task"] = job->active_task;
        j["task_size"] = job->task_size;
        j["first_frame"] = job->first_frame;
        j["last_frame"] = job->last_frame;

        __jobs[job->name] = j;
    }

    return jots(__jobs);
}

QString manager::pivot_to_server(QJsonArray recv)
{
    QString host = recv[0].toString();
    QJsonArray pks = recv[1].toArray();

    return tcpClient(host, SERVER_PORT, jats(pks));
}

void manager::reactive_all()
{
    // si el manager se apaga cuando esta rendereando se resetea todo a cola
    for (auto &job : *_jobs->get_items())
    {
        if (job->status == "Rendering...")
            job->status = "Queue";

        for (auto &task : job->task)
            if (task->status == "active")
            {
                task->status = "waiting";
                job->active_task -= 1;
            }
    }

    for (auto &server : *_servers->get_items())
        for (auto &instance : server->instances)
        {
            if (instance->status == 1)
                instance->status = 0;
            instance->reset = 0;
        }
}

QString manager::send_to_monitor_thread()
{
    // Envia informacion de los 'jobs' al vmonitor
    return jots(struct_to_json(_jobs, _servers, _groups));
}

void manager::update_all()
{
    while (true)
    {
        mutex->lock();

        _jobs->update();
        container_save();
        _servers->update();
        _groups->update();

        mutex->unlock();

        sleep(1);
    }
}

void manager::container_save()
{
    static int sec;
    sec++;

    // guarda informacion a json cada 10 seg
    if (sec > 10)
    {
        jwrite(VINARENDER_CONF_PATH + "/info.json",
               struct_to_json(_jobs, _servers, _groups));
        sec = 0;
    }
}

QString manager::recieve_monitor_thread(QJsonArray recv)
{
    QString id = recv[0].toString();
    QJsonArray pks = recv[1].toArray();

    if (not recv.empty())
    {
        if (id == "jobAction")
            _jobs->job_action(pks);

        if (id == "job_options")
            return _jobs->update_options(recv[1].toObject());

        if (id == "serverAction")
            return _servers->server_action(pks);

        if (id == "groupAction")
            _groups->group_action(pks);

        if (id == "taskAction")
            _tasks->task_action(pks);

        if (id == "groupCreate")
            _groups->group_create(pks);

        if (id == "settings")
            return settings_action(pks);
    }

    return "";
}

QString manager::settings_action(QJsonArray _pks)
{
    QString action = _pks[0].toString();

    if (action == "read")
        return jots(jread(VINARENDER_CONF_PATH + "/settings.json"));
    else
    {
        QJsonObject pks = _pks[1].toObject();
        settings->insert("paths", pks);
        jwrite(VINARENDER_CONF_PATH + "/settings.json", *settings);
    }

    return "";
}
