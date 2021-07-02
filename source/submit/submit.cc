#include "submit.h"
#include "../global/global.h"

void submit::init(int argc, char const *argv[])
{
    QJsonObject data = jofs(argv[1]);
    QString _host = data["host"].toString();

    QJsonObject settings = jread(VINARENDER_CONF_PATH + "/settings.json");
    int port = settings["manager"].toObject()["port"].toInt();
    QString host = settings["current_manager"].toString();

    if (!_host.isEmpty())
        host = _host;

    if (argc > 2)
        tcpClient(host, port, jats({4, data}));
}
