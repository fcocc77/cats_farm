#include <manager.h>

manager::manager()
{
    // Carga la informacion guardada de Jobs, Servers y Groups
    QJsonObject info = jread(VINARENDER_CONF_PATH + "/info.json");
    if (not info.empty())
        json_to_struct(info);
    //-----------------------------------------------
    reactive_all();
    // Recive la informacion del suministrador y crea un jobs con sus tareas

    // obtiene los puertos del manager y server
    settings = jread(VINARENDER_CONF_PATH + "/settings.json");
    int port = settings["manager"].toObject()["port"].toInt();
    server_port = settings["server"].toObject()["port"].toInt();
    // -------------------------------

    tcpServer(port, &manager::server_tcp, this);
    threading(&manager::update_all, this);
    threading(&manager::render_job, this);
}

QString manager::server_tcp(QString _recv)
{

    QJsonArray recv = jafs(_recv);
    int input = recv[0].toInt();
    QJsonArray pks = recv[1].toArray();

    QString send;

    mutex.lock();
    if (input == 1)
        send = update_server_thread(pks);
    else if (input == 2)
        send = send_to_monitor_thread();
    else if (input == 3)
        send = recieve_monitor_thread(pks);
    else if (input == 4)
        send = make_job(pks);
    else if (input == 5)
        send = pivot_to_server(pks);
    else if (input == 6)
        send = send_to_logger();
    mutex.unlock();
    if (input == 7)
        videovina(pks);

    return send;
}

QString manager::send_to_logger()
{
    QJsonObject _jobs;
    for (auto job : jobs)
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
        j["suspended_task"] = job->suspended_task;
        j["failed_task"] = job->failed_task;
        j["active_task"] = job->active_task;
        j["task_size"] = job->task_size;
        j["first_frame"] = job->first_frame;
        j["last_frame"] = job->last_frame;

        _jobs[job->name] = j;
    }

    return jots(_jobs);
} //-----------------------------------------

QString manager::pivot_to_server(QJsonArray recv)
{
    QString host = recv[0].toString();
    QJsonArray pks = recv[1].toArray();

    return tcpClient(host, server_port, jats(pks));
}

void manager::reactive_all()
{

    // si el manager se apaga cuando esta rendereando se resetea todo a cola
    for (auto &job : jobs)
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

    for (auto &server : servers)
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
    return jots(struct_to_json());
}

void manager::json_to_struct(QJsonObject info)
{
    for (auto j : info["jobs"].toObject())
    {
        job_struct *_jobs = new job_struct;
        QJsonObject job = j.toObject();
        _jobs->name = job["name"].toString();
        _jobs->status = job["status"].toString();
        _jobs->priority = job["priority"].toInt();
        for (QJsonValue _server : job["server"].toArray())
            _jobs->server.push_back(_server.toString());
        for (QJsonValue _group : job["server_group"].toArray())
            _jobs->server_group.push_back(_group.toString());
        _jobs->instances = job["instances"].toInt();
        _jobs->comment = job["comment"].toString();
        _jobs->submit_start = job["submit_start"].toString();
        _jobs->submit_finish = job["submit_finish"].toString();
        _jobs->time_elapsed = job["time_elapsed"].toInt();
        _jobs->last_time = job["last_time"].toInt();
        _jobs->total_render_time = job["total_render_time"].toString();
        _jobs->estimated_time = job["estimated_time"].toString();
        _jobs->time_elapsed_running = job["time_elapsed_running"].toBool();
        _jobs->software = job["software"].toString();
        _jobs->project = job["project"].toString();
        _jobs->system = job["system"].toString();
        _jobs->extra = job["extra"].toString();
        _jobs->errors = job["errors"].toInt();
        _jobs->render = job["render"].toString();

        for (QJsonValue vs : job["vetoed_servers"].toArray())
            _jobs->vetoed_servers.push_back(vs.toString());

        _jobs->progres = job["progres"].toInt();
        _jobs->waiting_task = job["waiting_task"].toInt();
        _jobs->tasks = job["tasks"].toInt();
        _jobs->suspended_task = job["suspended_task"].toInt();
        _jobs->failed_task = job["failed_task"].toInt();
        _jobs->active_task = job["active_task"].toInt();
        _jobs->task_size = job["task_size"].toInt();
        _jobs->first_frame = job["first_frame"].toInt();
        _jobs->last_frame = job["last_frame"].toInt();

        for (QJsonValue _t : job["task"].toArray())
        {
            task_struct *_tasks = new task_struct;
            QJsonObject t = _t.toObject();
            _tasks->name = t["name"].toString();
            _tasks->status = t["status"].toString();
            _tasks->first_frame = t["first_frame"].toInt();
            _tasks->last_frame = t["last_frame"].toInt();
            _tasks->server = t["server"].toString();
            _tasks->time = t["time"].toString();
            _jobs->task.push_back(_tasks);
        }

        jobs.push_back(_jobs);
    }

    for (QJsonValue s : info["servers"].toObject())
    {
        QJsonObject server = s.toObject();
        server_struct *_server = new server_struct;
        _server->name = server["name"].toString();
        _server->status = server["status"].toString();
        _server->host = server["host"].toString();
        _server->system = server["system"].toString();
        _server->cpu = server["cpu"].toInt();
        _server->cpu_iowait = server["cpu_iowait"].toInt();
        _server->cpu_cores = server["cpu_cores"].toInt();
        _server->ram = server["ram"].toInt();
        _server->ram_used = server["ram_used"].toDouble();
        _server->ram_cached = server["ram_cached"].toInt();
        _server->ram_total = server["ram_total"].toDouble();
        _server->temp = server["temp"].toInt();
        _server->mac = server["mac"].toString();
        _server->response_time = server["response_time"].toInt();

        for (QJsonValue i : server["instances"].toArray())
        {
            QJsonArray ins = i.toArray();
            inst_struct *_ins = new inst_struct;
            _ins->index = ins[0].toInt();
            _ins->status = ins[1].toInt();
            _ins->reset = ins[2].toBool();
            _ins->job_task = ins[3].toString();

            _server->instances.push_back(_ins);
        }

        _server->max_instances = server["max_instances"].toInt();
        _server->sshUser = server["sshUser"].toString();
        _server->sshPass = server["sshPass"].toString();
        _server->vmSoftware = server["vmSoftware"].toString();

        servers.push_back(_server);
    }

    for (QJsonValue g : info["groups"].toObject())
    {
        QJsonObject group = g.toObject();
        group_struct *_group = new group_struct;
        _group->name = group["name"].toString();
        _group->status = group["status"].toBool();
        _group->totaMachine = group["totaMachine"].toInt();
        _group->activeMachine = group["activeMachine"].toInt();

        for (QJsonValue s : group["server"].toArray())
        {
            QJsonArray server = s.toArray();
            serverFromGroupStruct *_server = new serverFromGroupStruct;
            _server->name = server[0].toString();
            _server->status = server[1].toBool();
            _group->server.push_back(_server);
        }

        groups.push_back(_group);
    }
}

QJsonObject manager::struct_to_json()
{
    // combierte todas las estructuras de Jobs y las combierte a JSON para poder
    // enviarlas y guardarlas
    QJsonObject info;

    QJsonObject _jobs;
    for (auto job : jobs)
    {
        QJsonObject j;

        j["name"] = job->name;
        j["status"] = job->status;
        j["priority"] = job->priority;
        // --------------------------------
        QJsonArray serverList;
        for (QString _server : job->server)
            serverList.push_back(_server);
        j["server"] = serverList;
        // --------------------------------
        QJsonArray serverGroup;
        for (QString _group : job->server_group)
            serverGroup.push_back(_group);
        j["server_group"] = serverGroup;
        // --------------------------------
        j["instances"] = job->instances;
        j["comment"] = job->comment;
        j["submit_start"] = job->submit_start;
        j["submit_finish"] = job->submit_finish;
        j["time_elapsed"] = job->time_elapsed;
        j["last_time"] = job->last_time;
        j["total_render_time"] = job->total_render_time;
        j["estimated_time"] = job->estimated_time;
        j["time_elapsed_running"] = job->time_elapsed_running;
        j["software"] = job->software;
        j["project"] = job->project;
        j["system"] = job->system;
        j["extra"] = job->extra;
        j["errors"] = job->errors;
        // --------------------------------
        QJsonArray vetoed_servers;
        for (QString s : job->vetoed_servers)
            vetoed_servers.push_back(s);
        j["vetoed_servers"] = vetoed_servers;
        // --------------------------------
        j["render"] = job->render;
        j["progres"] = job->progres;
        j["waiting_task"] = job->waiting_task;
        j["tasks"] = job->tasks;
        j["suspended_task"] = job->suspended_task;
        j["failed_task"] = job->failed_task;
        j["active_task"] = job->active_task;
        j["task_size"] = job->task_size;
        j["first_frame"] = job->first_frame;
        j["last_frame"] = job->last_frame;

        QJsonArray _tasks;
        for (auto task : job->task)
        {
            QJsonObject t;
            t["name"] = task->name;
            t["status"] = task->status;
            t["first_frame"] = task->first_frame;
            t["last_frame"] = task->last_frame;
            t["server"] = task->server;
            t["time"] = task->time;

            _tasks.push_back(t);
        }
        j["task"] = _tasks;

        _jobs[job->name] = j;
    }

    info["jobs"] = _jobs;
    //------------------------------------------------------------------------------

    QJsonObject _servers;
    for (auto server : servers)
    {
        QJsonObject s;
        s["name"] = server->name;
        s["status"] = server->status;
        s["host"] = server->host;
        s["system"] = server->system;
        s["cpu"] = server->cpu;
        s["cpu_iowait"] = server->cpu_iowait;
        s["cpu_cores"] = server->cpu_cores;
        s["ram"] = server->ram;
        s["ram_cached"] = server->ram_cached;
        s["ram_used"] = server->ram_used;
        s["ram_total"] = server->ram_total;
        s["temp"] = server->temp;
        s["mac"] = server->mac;
        s["response_time"] = server->response_time;
        // --------------------------------
        QJsonArray _instances;
        for (auto instance : server->instances)
            _instances.push_back({{instance->index, instance->status,
                                   instance->reset, instance->job_task}});
        s["instances"] = _instances;
        // --------------------------------
        s["max_instances"] = server->max_instances;
        s["sshUser"] = server->sshUser;
        s["sshPass"] = server->sshPass;
        s["vmSoftware"] = server->vmSoftware;
        s["log"] = server->log;

        _servers[server->name] = s;
    }

    info["servers"] = _servers;
    //------------------------------------------------------------------------------

    QJsonObject _groups;

    for (auto group : groups)
    {
        QJsonObject g;
        g["name"] = group->name;
        g["status"] = group->status;
        g["totaMachine"] = group->totaMachine;
        g["activeMachine"] = group->activeMachine;
        QJsonArray serverList;
        for (auto server : group->server)
            serverList.push_back({{server->name, server->status}});
        g["server"] = serverList;

        _groups[group->name] = g;
    }

    info["groups"] = _groups;

    return info;
}
