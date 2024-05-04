// Author: Francisco Contreras
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#include <QVBoxLayout>
#include <QJsonObject>

#include "layout_widget.h"
#include "util.h"
#include "../global/global.h"

layout_widget::layout_widget(QWidget *_properties, QWidget *_jobs,
                             QWidget *_tasks, QWidget *_groups,
                             QWidget *_servers, toolbar_class *_toolbar)
    : properties(_properties)
    , jobs(_jobs)
    , tasks(_tasks)
    , groups(_groups)
    , servers(_servers)
    , toolbar(_toolbar)
{

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);

    // Splitter
    QSplitter *splitter_main = new QSplitter(this);
    splitter_main->setOrientation(Qt::Vertical);

    QSplitter *splitter_top = new QSplitter(this);
    QSplitter *splitter_bottom = new QSplitter(this);

    // Layout
    splitter_top->addWidget(properties);
    splitter_top->addWidget(jobs);
    splitter_top->addWidget(tasks);

    splitter_bottom->addWidget(servers);
    splitter_bottom->addWidget(groups);

    splitter_main->addWidget(splitter_top);
    splitter_main->addWidget(splitter_bottom);

    // Tamanio de Splitters
    splitter_main->setSizes({10, 100, 10});
    splitter_top->setSizes({120, 300, 120});
    splitter_bottom->setSizes({300, 120});

    layout->addWidget(splitter_main);

    restore_layout();
}

void layout_widget::save_layout()
{
    QJsonObject layout = {
        {"properties", properties->isVisible()},
        {"tasks", toolbar->get_tasks_switch()->isChecked()},
        {"groups", toolbar->get_groups_switch()->isChecked()},
        {"servers", toolbar->get_servers_switch()->isChecked()}};

    jwrite(VINARENDER_CONF_PATH + "/layout.json", layout);
}

void layout_widget::restore_layout() {

    QJsonObject layout = jread(VINARENDER_CONF_PATH + "/layout.json");

    toolbar->set_checked_tasks(layout["tasks"].toBool());
    toolbar->set_checked_groups(layout["groups"].toBool());
    toolbar->set_checked_servers(layout["servers"].toBool());
}
