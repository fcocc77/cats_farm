#include "servers.h"
#include "threading.h"
#include "os.h"
#include "items_util.h"
#include "manager.h"
#include "tcp.h"

servers::servers(void *__manager)
    : _manager(__manager)
    , items(new QList<server_struct *>)
{
    manager *___manager = static_cast<manager *>(_manager);
    preferences = ___manager->get_preferences();
    settings = ___manager->get_settings();

    server_port = settings->value("server").toObject()["port"].toInt();
}

QString servers::update_server_thread(QJsonArray recv)
{
    if (not recv.empty())
    {
        QString name = recv[0].toString();
        QString host = recv[1].toString();
        int cpu = recv[2].toInt();
        int cpu_iowait = recv[3].toInt();
        int ram = recv[4].toInt();
        int ram_cached = recv[5].toInt();
        int temp = recv[6].toInt();
        QString system = recv[7].toString();
        QString mac = recv[8].toString();
        float ram_total = recv[9].toDouble();
        float ram_used = recv[10].toDouble();
        int cpu_cores = recv[11].toInt();
        QString log = recv[12].toString();
        QString sshUser = recv[13].toString();
        QString sshPass = recv[14].toString();

        int response_time = time(0);

        if (not contains(items, name))
        {
            QList<inst_struct *> instances;

            for (int i = 0; i < 16; ++i)
            {
                inst_struct *ins = new inst_struct;
                ins->index = i;
                ins->status = 0;
                ins->reset = 0;
                ins->job_task = "...";
                instances.push_back(ins);
            }

            server_struct *server = new server_struct;

            server->name = name;
            server->status = "idle";
            server->host = host;
            server->system = system;
            server->cpu = cpu;
            server->cpu_iowait = cpu_iowait;
            server->cpu_cores = cpu_cores;
            server->ram = ram;
            server->ram_cached = ram_cached;
            server->ram_used = ram_used;
            server->ram_total = ram_total;
            server->temp = temp;
            server->mac = mac;
            server->response_time = response_time;
            server->instances = instances;
            server->max_instances = 1;
            server->sshUser = sshUser;
            server->sshPass = sshPass;
            server->log = log;
            items->push_back(server);
        }

        else
        {
            auto server = get_server(name);

            server->ram_used = ram_used;
            server->cpu = cpu;
            server->cpu_iowait = cpu_iowait;
            server->ram = ram;
            server->ram_cached = ram_cached;
            server->temp = temp;
            server->host = host;
            server->log = log;
            server->response_time = response_time;
        }
    }

    // se usa mutex para que cuando se este generando el json no se este
    // copiando informacion en "preferences" si no el programa se cae
    QString ret;

    ret = jots(*preferences);

    return ret;
}

void servers::update()
{
    int t1 = time(0);

    QJsonArray serverList;

    for (auto server : *items)
    {
        serverList.push_back(server->name);

        // set status server
        int t2 = server->response_time;
        int response_time = t1 - t2;

        // si el porcentaje de la ram es mayor a 90 %, el tiempo de espera es
        // mas para que no quede aucente le servidor
        int response;
        if (server->ram > 90)
            response = 120;
        else
            response = 30;

        // si no responde en 10 segundos, queda ausente
        if (response_time > response)
            server->status = "absent";
        else
            server->status = "idle";

        for (int i = 0; i < server->max_instances; ++i)
        {
            if (server->instances[i]->status == 3)
                threading([=]() {
                    sleep(7);
                    if (server->instances[i]->status == 3)
                    {
                        server->instances[i]->status = 0;
                        server->instances[i]->job_task = "...";
                    }
                });
        }
    }

    preferences->insert("servers", serverList);
}

void servers::reset_all_servers()
{
    // Resetea todos los servidores para que empiece otra vez
    for (auto server : *items)
        for (int i = 0; i < server->max_instances; i++)
            server->instances[i]->reset = 1;

    static_cast<manager *>(_manager)->get_renderer()->to_reset_render();
}


QString servers::server_action(QJsonArray pks)
{
    for (QJsonValue s : pks)
    {
        QJsonArray _server = s.toArray();
        QString name = _server[0].toString();
        QString server_action = _server[1].toString();
        int instances = _server[2].toString().toInt();
        auto server = get_server(name);

        if (server_action == "turn_on")
            os::sh("ether-wake " + server->mac);

        if (server_action == "max_instances")
            server->max_instances = instances;

        if (server_action == "inactive")
            server_set_state(server, false);

        if (server_action == "reactive")
            server_set_state(server, true);

        if (server_action == "delete")
            if (not(server->status == "active"))
                erase_by_name(items, name);

        if (server_action == "ssh")
            return jats({server->sshUser, server->sshPass, server->host});
    }

    return "{}";
}

void servers::server_set_state(server_struct *server, bool state)
{

    if (state)
    {
        for (int i = 0; i < server->max_instances; ++i)
        {
            auto ins = server->instances[i];
            if (ins->status == 2)
            {
                ins->status = 0;
                ins->job_task = "...";
            }
        }
    }

    else
    {
        QJsonArray kill_ins;

        for (int i = 0; i < server->max_instances; ++i)
        {
            auto ins = server->instances[i];
            ins->status = 2;
            ins->job_task = "...";
            kill_ins.push_back(i);
        }

        tcpClient(server->host, server_port, jats({3, kill_ins}));
    }
}

server_struct *servers::get_server(QString name)
{
    for (auto server : *items)
        if (server->name == name)
            return server;

    return items->value(0);
}
