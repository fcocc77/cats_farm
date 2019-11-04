#include "logger.hpp"

logger::logger()
{
    // el json zones.json es generado al iniciar el servior de django,
    // esta informacion esta declarada en general.py render_zones.
    zones = jread(path + "/etc/zones.json");

    for (QString key : zones.keys())
    {
        QString ip = zones[key].toString();
        tcpClient(ip, 7000, &logger::get, this, false, {{"zone", key}});
    }
}

QString logger::get(QString _recv, QJsonObject extra)
{
    QString zone = extra["zone"].toString();
    QString zones_dir = path + "/etc/zones";

    if (!os::isdir(zones_dir))
        os::makedirs(zones_dir);

    QJsonObject jobs = jofs(_recv);
    QJsonObject zone_data = {
        {"count", jobs.count()},
        {"jobs", jobs}
    };

    jwrite(zones_dir + "/" + zone + ".json", zone_data);
    return jats({6});
}