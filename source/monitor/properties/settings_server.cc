#include <QJsonObject>
#include <QVBoxLayout>

#include "settings_server.h"
#include "util.h"
#include "../global/global.h"
#include "settings.h"

settings_server::settings_server(shared_variables *_shared,
                                 QWidget *_settings_widget)
    : shared(_shared)
    , settings_widget(_settings_widget)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    manager_host = new combo_box;

    layout->addWidget(manager_host);
}

void settings_server::update_ips()
{
    manager_host->clear();

    QJsonArray hosts = static_cast<settings_class *>(settings_widget)
                           ->get_monitor()
                           ->get_hosts();

    for (QJsonValue host : hosts)
        manager_host->add_item(host.toString());

    manager_host->set_current_text(current_ip);
}

void settings_server::save()
{
    QJsonObject server = shared->settings["server"].toObject();
    server["handler_ip"] = manager_host->get_current_text();
    shared->settings["server"] = server;

    jwrite(VINARENDER_CONF_PATH + "/settings.json", shared->settings);
    os::system("systemctl restart vserver");
}

void settings_server::restore()
{
    current_ip = shared->settings["server"].toObject()["handler_ip"].toString();
    update_ips();
}
