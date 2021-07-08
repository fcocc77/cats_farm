#include <QJsonObject>
#include <QVBoxLayout>

#include "settings_server.h"
#include "util.h"
#include "../global/global.h"

settings_server::settings_server(shared_variables *_shared)
    : shared(_shared)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    manager_host = new text_knob("Manager IP");

    layout->addWidget(manager_host);
}

void settings_server::save()
{
    QJsonObject server = shared->settings["server"].toObject();
    server["handler_ip"] = manager_host->get_text();
    shared->settings["server"] = server;

    jwrite(VINARENDER_CONF_PATH + "/settings.json", shared->settings);
    os::system("systemctl restart vserver");
}

void settings_server::restore()
{
    QString host = shared->settings["server"].toObject()["handler_ip"].toString();
    manager_host->set_text(host);
}
