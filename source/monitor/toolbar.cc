#include <toolbar.h>

toolbar_class::toolbar_class(QMainWindow *_monitor, global_class *_global,
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

    resume = new QPushButton("RESUME JOB");
    resume->setObjectName("resume");
    main_layout->addWidget(resume);

    suspend = new QPushButton("SUSPEND JOB");
    suspend->setObjectName("suspend");
    main_layout->addWidget(suspend);

    settings = new QPushButton("SETTINGS");
    settings->setObjectName("settings");
    main_layout->addWidget(settings);

    QLabel *zone = new QLabel("ZONE:");
    zone->setObjectName("zone");
    main_layout->addWidget(zone);

    shared->zone_box = new QComboBox();
    shared->zone_box->setObjectName("zone_box");
    main_layout->addWidget(shared->zone_box);

    shared->conection = new QLabel();
    shared->conection->setObjectName("conection");
    main_layout->addWidget(shared->conection);

    QLabel *jobs_label = new QLabel("QUEUE JOBS:");
    jobs_label->setObjectName("jobs_label");
    main_layout->addWidget(jobs_label);

    shared->jobs_count = new QLabel("0 jobs.");
    shared->jobs_count->setObjectName("jobs_count");
    main_layout->addWidget(shared->jobs_count);

    main_layout->addStretch();

    jobs_switch = new QPushButton("Jobs");
    jobs_switch->setObjectName("jobs_switch");
    main_layout->addWidget(jobs_switch);

    tasks_switch = new QPushButton("Tasks");
    tasks_switch->setObjectName("tasks_switch");
    main_layout->addWidget(tasks_switch);

    servers_switch = new QPushButton("Servers");
    servers_switch->setObjectName("servers_switch");
    main_layout->addWidget(servers_switch);

    groups_switch = new QPushButton("Groups");
    groups_switch->setObjectName("groups_switch");
    main_layout->addWidget(groups_switch);

    log_switch = new QPushButton("Log");
    log_switch->setObjectName("log_switch");
    main_layout->addWidget(log_switch);
}

void toolbar_class::connections()
{
    connect(settings, &QPushButton::clicked, this, [this]() {
        monitor->setUpdatesEnabled(false);
        properties->switch_widget("settings");
        monitor->setUpdatesEnabled(true);
    });

    connect(suspend, &QPushButton::clicked, this,
            [this]() { jobs->job_suspend_action->triggered(); });

    connect(resume, &QPushButton::clicked, this,
            [this]() { jobs->job_resume_action->triggered(); });

    connect(shared->zone_box, &QComboBox::currentTextChanged, update,
            &update_class::update);

    connect(jobs_switch, &QPushButton::clicked, this, [this]() {
        monitor->setUpdatesEnabled(false);
        jobs->parentWidget()->setVisible(!jobs->parentWidget()->isVisible());
        monitor->setUpdatesEnabled(true);
    });

    connect(tasks_switch, &QPushButton::clicked, this, [this]() {
        monitor->setUpdatesEnabled(false);
        tasks->tree->parentWidget()->setVisible(
            !tasks->tree->parentWidget()->isVisible());
        monitor->setUpdatesEnabled(true);
    });

    connect(servers_switch, &QPushButton::clicked, this, [this]() {
        monitor->setUpdatesEnabled(false);
        servers->parentWidget()->setVisible(
            !servers->parentWidget()->isVisible());
        monitor->setUpdatesEnabled(true);
    });
    connect(groups_switch, &QPushButton::clicked, this, [this]() {
        monitor->setUpdatesEnabled(false);
        groups->parentWidget()->setVisible(
            !groups->parentWidget()->isVisible());
        monitor->setUpdatesEnabled(true);
    });
    connect(log_switch, &QPushButton::clicked, this, [this]() {
        monitor->setUpdatesEnabled(false);
        properties->switch_widget("log");
        monitor->setUpdatesEnabled(true);
    });
}

void toolbar_class::load_zones()
{
    QJsonArray zones = shared->settings["hosts"].toArray();
    QString current_manager = shared->settings["current_manager"].toString();

    // agrega todas las zonas guardadas al combobox
    for (QJsonValue value : zones)
    {
        QString zone = value.toString();
        shared->zone_box->addItem(zone);
    }
    // ---------------------------

    // check zona actual en el combobox
    shared->zone_box->setCurrentText(current_manager);
    // ----------------------
    // establece por defecto la ultima zona guardada
    update->update(current_manager);
    // ----------------------
}
