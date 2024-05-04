// Author: Francisco Contreras
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#include <QPushButton>
#include <QVBoxLayout>

#include "../global/global.h"
#include "settings_monitor.h"
#include "submit_global.h"

settings_monitor::settings_monitor(shared_variables *_shared)
    : shared(_shared)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    layout->setMargin(0);

    QWidget *tabs_widget = new QWidget;
    tabs_widget->setObjectName("tabs_widget");
    QVBoxLayout *tabs_layout = new QVBoxLayout(tabs_widget);
    tabs_layout->setMargin(0);

    managers_tree = new QTreeWidget();
    QWidget *maker_widget = new QWidget;
    QHBoxLayout *maker_layout = new QHBoxLayout(maker_widget);

    maker_name_edit = new QLineEdit;
    QPushButton *maker_add_button = new QPushButton("add");
    QPushButton *maker_remove_button = new QPushButton("remove");

    submit_ip = new combo_box();
    QLabel *submit_ip_label = new QLabel("Submit Manager IP:");

    // Ajustes"
    maker_name_edit->setPlaceholderText("Manager IP: 0.0.0.0");
    managers_tree->setMaximumHeight(100);

    // Conecciones
    connect(maker_add_button, &QPushButton::clicked, this,
            &settings_monitor::add_manager);

    connect(maker_remove_button, &QPushButton::clicked, this,
            &settings_monitor::remove_manager);

    // Layout
    maker_layout->addWidget(maker_name_edit);
    maker_layout->addWidget(maker_add_button);
    maker_layout->addWidget(maker_remove_button);

    layout->addWidget(managers_tree);
    layout->addWidget(maker_widget);
    layout->addWidget(submit_ip_label);
    layout->addWidget(submit_ip);
}

QJsonArray settings_monitor::get_hosts() const
{
    QJsonArray json_hosts;
    for (int i = 0; i < managers_tree->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item = managers_tree->topLevelItem(i);
        json_hosts.push_back(item->text(0));
    }

    return json_hosts;
}

void settings_monitor::add_manager()
{
    QString ip = maker_name_edit->text();
    QStringList sections = ip.split('.');

    if (sections.count() != 4)
        return;

    for (QChar c : ip)
        if (!c.isDigit() && c != '.')
            return;

    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(0, ip);
    managers_tree->addTopLevelItem(item);
}

void settings_monitor::remove_manager()
{
    if (!managers_tree->selectedItems().count())
        return;

    delete managers_tree->selectedItems()[0];
}

void settings_monitor::update_submit_ip()
{
    submit_ip->clear();

    for (QJsonValue host : get_hosts())
        submit_ip->add_item(host.toString());

    submit_ip->set_current_text(current_submit_ip);
}

void settings_monitor::save()
{
    QJsonObject monitor_settings = shared->settings["monitor"].toObject();
    monitor_settings["hosts"] = get_hosts();
    shared->settings["monitor"] = monitor_settings;

    QJsonObject submit = shared->settings["submit"].toObject();
    submit["manager_ip"] = submit_ip->get_current_text();
    shared->settings["submit"] = submit;

    jwrite(VINARENDER_CONF_PATH + "/settings.json", shared->settings);

    // agrega las ips al combobox de zonas
    shared->zone_box->clear();
    for (QJsonValue ip : get_hosts())
        shared->zone_box->add_item(ip.toString());
}

void settings_monitor::restore()
{
    managers_tree->clear();

    QJsonObject monitor_settings = shared->settings["monitor"].toObject();

    for (QJsonValue host : monitor_settings["hosts"].toArray())
    {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, host.toString());
        managers_tree->addTopLevelItem(item);
    }

    current_submit_ip =
        shared->settings["submit"].toObject()["manager_ip"].toString();

    update_submit_ip();
}
