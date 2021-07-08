#include <QVBoxLayout>

#include "settings_monitor.h"
#include "../global/global.h"
#include "submit_global.h"

settings_monitor::settings_monitor(shared_variables *_shared)
    : shared(_shared)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);

    QWidget *hosts_widget = new QWidget;
    hosts_widget->setObjectName("hosts_widget");
    QVBoxLayout *hosts_layout = new QVBoxLayout(hosts_widget);
    hosts_layout->setMargin(0);
    hosts_layout->setSpacing(0);

    zones_edit = new text_knob("Manager ZONES");

    int h = HORIZONTAL_MARGIN;
    int v = VERTICAL_MARGIN;
    int s = SPACING;

    zones_edit->layout()->setContentsMargins(h, v, h, s);

    QWidget *tabs_widget = new QWidget;
    tabs_widget->setObjectName("tabs_widget");
    QVBoxLayout *tabs_layout = new QVBoxLayout(tabs_widget);

    tabs_layout->setMargin(0);

    // Layout
    hosts_layout->addWidget(zones_edit);

    layout->addWidget(hosts_widget);
}

QJsonArray settings_monitor::get_hosts() const
{
    QString zones = zones_edit->get_text();

    QJsonArray json_hosts;
    for (QString zone : zones.split(","))
        json_hosts.push_back(zone.replace(" ", ""));

    return json_hosts;
}

void settings_monitor::save()
{

    QJsonObject monitor_settings = shared->settings["monitor"].toObject();
    monitor_settings["hosts"] = get_hosts();

    shared->settings["monitor"] = monitor_settings;

    jwrite(VINARENDER_CONF_PATH + "/settings.json", shared->settings);

    // agrega las ips al combobox de zonas
    shared->zone_box->clear();
    for (QJsonValue ip : get_hosts())
        shared->zone_box->add_item(ip.toString());
}

void settings_monitor::restore()
{
    QJsonObject monitor_settings = shared->settings["monitor"].toObject();

    QString hosts;
    for (QJsonValue host : monitor_settings["hosts"].toArray())
        hosts += host.toString() + ", ";
    hosts = hosts.left(hosts.length() - 2);

    zones_edit->set_text(hosts);

}
