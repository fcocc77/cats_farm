#include "submit.h"
#include "../global/global.h"
#include "util.h"

void submit::init(int argc, char const *argv[])
{
    QJsonObject data = jofs(argv[1]);

    QJsonObject settings = jread(VINARENDER_CONF_PATH + "/settings.json");
    int port = settings["manager"].toObject()["port"].toInt();
    QString host = settings["current_manager"].toString();

    tcpClient(host, port, jats({4, data}));
}
