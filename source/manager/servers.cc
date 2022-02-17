// Author: Francisco Jose Contreras Cuevas
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

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
    settings = ___manager->get_settings();
}

QString servers::update_server_thread(QJsonObject recv)
{
    if (not recv.empty())
    {
        QString hostname = recv["hostname"].toString();
        QString ip = recv["ip"].toString();
        QString mac_address = recv["mac_address"].toString();
        QString system = recv["system"].toString();

        int cpu_used = recv["cpu_used"].toInt();
        int cpu_iowait = recv["cpu_iowait"].toInt();
        int cpu_temp = recv["cpu_temp"].toInt();
        int cpu_cores = recv["cpu_cores"].toInt();

        int ram_usage_percent = recv["ram_usage_percent"].toInt();
        int ram_cache_percent = recv["ram_cache_percent"].toInt();
        float ram_total = recv["ram_total"].toDouble();
        float ram_used = recv["ram_used"].toDouble();

        int response_time = time(0);

        if (not contains(items, hostname))
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

            server->name = hostname;
            server->status = "idle";
            server->ip = ip;
            server->system = system;
            server->cpu_used = cpu_used;
            server->cpu_iowait = cpu_iowait;
            server->cpu_cores = cpu_cores;
            server->ram_usage_percent = ram_usage_percent;
            server->ram_cache_percent = ram_cache_percent;
            server->ram_used = ram_used;
            server->ram_total = ram_total;
            server->cpu_temp = cpu_temp;
            server->mac_address = mac_address;
            server->response_time = response_time;
            server->instances = instances;
            server->max_instances = 1;
            items->push_back(server);
        }

        else
        {
            auto server = get_server(hostname);

            server->ram_used = ram_used;
            server->cpu_used = cpu_used;
            server->cpu_iowait = cpu_iowait;
            server->ram_usage_percent = ram_usage_percent;
            server->ram_cache_percent = ram_cache_percent;
            server->cpu_temp = cpu_temp;
            server->ip = ip;
            server->response_time = response_time;
        }
    }

    // se usa mutex para que cuando se este generando el json no se este
    // copiando informacion en "settings" si no el programa se cae
    QString ret;

    ret = jots(*settings);

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
        if (server->ram_usage_percent > 90)
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

    settings->insert("servers", serverList);
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
            os::sh("ether-wake " + server->mac_address);

        if (server_action == "max_instances")
            server->max_instances = instances;

        if (server_action == "inactive")
            server_set_state(server, false);

        if (server_action == "reactive")
            server_set_state(server, true);

        if (server_action == "delete")
            if (not(server->status == "active"))
                erase_by_name(items, name);
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

        tcpClient(server->ip, SERVER_PORT, jats({3, kill_ins}));
    }
}

server_struct *servers::get_server(QString name)
{
    for (auto server : *items)
        if (server->name == name)
            return server;

    return items->value(0);
}
