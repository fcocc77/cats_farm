#include <QJsonObject>
#include <QVBoxLayout>

#include "settings_submit.h"
#include "util.h"
#include "../global/global.h"

settings_submit::settings_submit(shared_variables *_shared)
    : shared(_shared)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    manager_host = new text_knob("Manager IP");

    layout->addWidget(manager_host);
}

void settings_submit::save()
{
    QJsonObject server = shared->settings["submit"].toObject();
    server["manager_ip"] = manager_host->get_text();
    shared->settings["submit"] = server;

    jwrite(VINARENDER_CONF_PATH + "/settings.json", shared->settings);
}

void settings_submit::restore()
{
    QString host =
        shared->settings["submit"].toObject()["manager_ip"].toString();
    manager_host->set_text(host);
}
