#include <QAction>

#include "toolbar.h"

toolbar_class::toolbar_class(QMainWindow *_monitor, general_class *_global,
                             jobs_class *_jobs, tasks_class *_tasks,
                             servers_class *_servers, groups_class *_groups,
                             log_class *_log, update_class *_update,
                             shared_variables *_shared,
                             QWidget *_settings_widget,
                             properties_class *_properties)

    : monitor(_monitor)
    , global(_global)
    , jobs(_jobs)
    , tasks(_tasks)
    , servers(_servers)
    , groups(_groups)
    , log(_log)
    , update(_update)
    , shared(_shared)
    , settings_widget(_settings_widget)
    , properties(_properties)
{
    setup_ui();
    load_zones();
    connections();
}

toolbar_class::~toolbar_class() {}

void toolbar_class::setup_ui()
{
    QHBoxLayout *main_layout = new QHBoxLayout();
    this->setObjectName("tool_bar");
    this->setContentsMargins(0, 0, 0, 0);
    this->setLayout(main_layout);

    settings = new button("SETTINGS", "settings", true, false);
    submit = new button("SUBMIT", "submit", true, false);
    log_switch = new button("LOG", "log", true, false);
    options_button = new button("JOBS SETTINGS", "options", true, false);

    QLabel *zone = new QLabel("ZONE:");
    zone->setObjectName("zone");

    shared->zone_box = new combo_box();

    shared->conection = new QLabel();
    shared->conection->setObjectName("conection");

    QLabel *jobs_label = new QLabel("QUEUE JOBS:");
    jobs_label->setObjectName("jobs_label");

    shared->jobs_count = new QLabel("0 jobs.");
    shared->jobs_count->setObjectName("jobs_count");

    tasks_switch = new button("Tasks", "tasks", true);
    servers_switch = new button("Servers", "server", true);
    groups_switch = new button("Groups", "groups", true);

    // Layout
    main_layout->addWidget(settings);
    main_layout->addWidget(log_switch);
    main_layout->addWidget(options_button);
    main_layout->addWidget(submit);

    main_layout->addWidget(zone);
    main_layout->addWidget(shared->zone_box);
    main_layout->addWidget(shared->conection);
    main_layout->addWidget(jobs_label);
    main_layout->addWidget(shared->jobs_count);

    main_layout->addStretch();

    main_layout->addWidget(tasks_switch);
    main_layout->addWidget(servers_switch);
    main_layout->addWidget(groups_switch);
}

void toolbar_class::connections()
{
    connect(settings, &button::clicked, this,
            [this]() { switch_widget("settings"); });

    connect(submit, &button::clicked, this,
            [this]() { switch_widget("submit"); });

    connect(shared->zone_box, &combo_box::text_changed, update,
            &update_class::update);

    connect(tasks_switch, &button::clicked, this, [this]() {
        tasks->tree->parentWidget()->setVisible(
            !tasks->tree->parentWidget()->isVisible());
    });

    connect(servers_switch, &button::clicked, this, [this]() {
        servers->parentWidget()->setVisible(
            !servers->parentWidget()->isVisible());
    });

    connect(groups_switch, &button::clicked, this, [this]() {
        groups->parentWidget()->setVisible(
            !groups->parentWidget()->isVisible());
    });

    connect(log_switch, &button::clicked, this,
            [this]() { switch_widget("log"); });
}

void toolbar_class::switch_widget(QString widget_name)
{
    settings->set_checked(false);
    log_switch->set_checked(false);
    submit->set_checked(false);

    bool visible = properties->switch_widget(widget_name);

    if (widget_name == "settings")
        settings->set_checked(visible);
    else if (widget_name == "submit")
        submit->set_checked(visible);
    else if (widget_name == "log")
        log_switch->set_checked(visible);
}

void toolbar_class::hide_properties()
{
    properties->parentWidget()->hide();

    settings->set_checked(false);
    log_switch->set_checked(false);
    submit->set_checked(false);
}

void toolbar_class::load_zones()
{
    QJsonArray zones = shared->settings["hosts"].toArray();
    QString current_manager = shared->settings["current_manager"].toString();

    // agrega todas las zonas guardadas al combobox
    for (QJsonValue value : zones)
    {
        QString zone = value.toString();
        shared->zone_box->add_item(zone);
    }
    // ---------------------------

    // check zona actual en el combobox
    shared->zone_box->set_current_text(current_manager);
    // ----------------------
    // establece por defecto la ultima zona guardada
    update->update(current_manager);
    // ----------------------
}
