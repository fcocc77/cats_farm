#include <logger.h>
#include "../global/global.h"

logger::logger()
{
    int port = jread(VINARENDER_CONF_PATH + "/settings.json")["manager"]
                   .toObject()["port"]
                   .toInt();

    // el json zones.json es generado al iniciar el servior de django,
    // esta informacion esta declarada en general.py render_zones.
    zones = jread(VINARENDER_CONF_PATH + "/zones.json");

    for (QString key : zones.keys())
    {
        QString ip = zones[key].toString();
        tcpClient(ip, port, &logger::get, this, false, {{"zone", key}});
    }

    // guarda un json cada 1 segundo
    threading(&logger::save, this);
    // -------------------------
}

void logger::save()
{
    while (1)
    {
        mutex.lock();
        jwrite(VINARENDER_CONF_PATH + "/zones_jobs.json", zones_jobs);
        mutex.unlock();

        sleep(1);
    }
}

QString logger::get(QString _recv, QJsonObject extra)
{
    QString zone = extra["zone"].toString();

    QJsonObject jobs = jofs(_recv);
    QJsonObject zone_data = {{"count", jobs.count()}, {"jobs", jobs}};

    mutex.lock();
    zones_jobs[zone] = zone_data;
    mutex.unlock();

    return jats({6});
}
