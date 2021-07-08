#include "submit.h"
#include "../global/global.h"
#include "util.h"

void submit::init(int argc, char const *argv[])
{
    QJsonObject data = jofs(argv[1]);

    QJsonObject settings = jread(VINARENDER_CONF_PATH + "/settings.json");
    QString host = settings["submit"].toObject()["manager_ip"].toString();

    tcpClient(host, MANAGER_PORT, jats({4, data}));
}
