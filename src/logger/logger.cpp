#include "logger.h"

logger::logger()
{
    // el json zones.json es generado al iniciar el servior de django,
    // esta informacion esta declarada en general.py render_zones.

    zones = jread(path + "/etc/zones.json");

    for (QString key : zones.keys())
    {
        QString zone = zones[key].toString();
        print(key);
    }

    QJsonObject extra = {{"zone", "mi_zona"}};
    tcpClient("192.168.1.77", 7000, &logger::get, this, false, extra);
}

QString logger::get(QString _recv, QJsonObject extra)
{
    QJsonObject recv = jofs(_recv);
    jwrite(path + "/etc/jobs.json", recv);
    return jats({6});
}