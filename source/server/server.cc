#include "server.h"
#include "../global/global.h"
#include <hardware_monitor.h>

server::server()
{
    render = new render_class(&mutex);

    // obtiene los puertos del manager y server
    QJsonObject settings = jread(VINARENDER_CONF_PATH + "/settings.json");
    QString manager_host =
        settings["server"].toObject()["handler_ip"].toString();

    tcpClient(manager_host, MANAGER_PORT, &server::send_resources, this);
    tcpServer(SERVER_PORT, &server::update_from_manager, this);
}

QString server::send_resources(QString recv, QJsonObject extra)
{
    if (not recv.isEmpty())
    {
        mutex.lock();
        render->settings_from_manager = jofs(recv);
        mutex.unlock();
        jwrite(VINARENDER_CONF_PATH + "/settings_from_manager.json",
               render->settings_from_manager);
    }

    QJsonObject server_info = {
        {"hostname", os::hostName()},
        {"ip", os::ip()},
        {"cpu_used", hm::get_cpu_used()},
        {"cpu_iowait", hm::get_iowait_cpu_used()},
        {"cpu_temp", hm::get_cpu_temp()},
        {"cpu_cores", hm::get_cpu_cores()},
        {"ram_total", hm::get_ram_total()},
        {"ram_usage", hm::get_ram_used()},
        {"ram_usage_percent", hm::get_ram_percent()},
        {"ram_cache_percent", hm::get_ram_percent(true)},
        {"system", _linux ? "Linux" : "Windows"},
        {"mac_address", os::mac()},
    };

    return jats({1, server_info});
}

QString server::update_from_manager(QString _recv)
{
    QJsonArray json = jafs(_recv);
    int input = json[0].toInt();
    QJsonArray recv = json[1].toArray();

    QString send;

    if (input == 0)
        send = render->render_task(json[1].toObject());

    if (input == 1)
        send = get_log();

    if (input == 3)
    {
        mutex.lock();
        for (auto i : recv)
        {
            int pid = render->idata[i.toInt()].pid;
            if (pid)
            {
                render->idata[i.toInt()].task_kill = true;
                os::kill(pid);
            }
        }
        mutex.unlock();

        // Mata las pid a partir del nombre "d" (es el after effect render de
        // wine)
        QStringList ps = os::sh("ps cax").split("\n");
        for (QString line : ps)
        {
            QString lastWord = line.split(" ").back();
            if (lastWord == "d")
            {
                QString pid = line.split(" ")[0];
                os::sh("kill " + pid);
            }
        }
    }

    if (input == 4)
    {
        QString action = recv[0].toString();
        QString info = recv[1].toString();

        if (action == "freeram")
            if (_linux)
                os::sh("sh -c \"sync && sysctl -w vm.drop_caches=3\"");
        if (action == "off")
            os::system("shutdown now");
    }

    return send;
}

QString server::get_log() const
{
    QJsonObject logs;

    for (QString software : {"nuke", "ffmpeg", "houdini"})
    {
        QString log;

        for (int i = 0; i < 4; i++)
        {
            log += fread(VINARENDER_PATH + "/log/" + software + '_' +
                         QString::number(i));

            log += "\n\n";
        }

        logs[software] = log;
    }

    return jots(logs);
}
