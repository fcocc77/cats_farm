#include "logger.h"

logger::logger()
{
    tcpClient(managerHost, 7000, &logger::get, this, true);
}

QString logger::get(QString _recv)
{
    QJsonObject recv = jofs(_recv);
    jwrite(path + "/etc/jobs.json", recv);
    return jats({6});
}