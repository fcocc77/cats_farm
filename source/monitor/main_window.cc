#include <QSplitter>
#include <QTextEdit>

#include "util.h"
#include "../global/global.h"
#include "main_window.h"

monitor::monitor(QWidget *parent)
    : QMainWindow(parent)
{
    setup_ui();
}

monitor::~monitor()
{
    QString openMonitor = VINARENDER_PATH + "/etc/openMonitor";

    // guarda un bool para ver si el monitor ya esta cerrado.
    fwrite(openMonitor, "0");
}

QWidget *monitor::add_title(QWidget *widget, QString title)
{
    // le agrega un titilo superior al 'widget'
    QWidget *container = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    container->setLayout(layout);

    QLabel *label = new QLabel(title);
    label->setObjectName("widget_title");
    layout->addWidget(label);
    layout->addWidget(widget);

    return container;
}

void monitor::setup_ui()
{
    shared = new shared_variables();
    shared->settings = jread(VINARENDER_PATH + "/etc/settings.json");
    shared->manager_host =
        shared->settings["manager"].toObject()["ip"].toString();
    shared->manager_port =
        shared->settings["manager"].toObject()["port"].toInt();
    shared->server_port = shared->settings["server"].toObject()["port"].toInt();

    // Splitter
    QSplitter *splitter_main = new QSplitter(this);
    splitter_main->setOrientation(Qt::Vertical);
    this->setCentralWidget(splitter_main);

    QSplitter *splitter_top = new QSplitter(this);
    QSplitter *splitter_bottom = new QSplitter(this);

    // Log
    log = new log_class();
    options = new options_class();
    settings = new settings_class(shared);

    // Submit
    _submit = new submit();

    // Widget de propiedades
    properties = new properties_class(log, options, settings, _submit);
    QWidget *properties_parent = add_title(properties, "Properties");
    properties_parent->hide();
    properties_parent->setMaximumWidth(500);

    // Servers
    servers = new servers_class(this, shared, log);
    QWidget *servers_parents = add_title(servers, "Servers");

    // Groups
    groups = new groups_class(this, shared, servers);
    QWidget *groups_parent = add_title(groups, "Groups");

    // Jobs
    jobs =
        new jobs_class(shared, this, log, servers, options, groups, properties);

    // Task
    tasks = new tasks_class(this, shared, jobs);
    QWidget *tasks_parent = add_title(tasks->tree, "Tasks");

    update =
        new update_class(shared, groups, jobs, servers, tasks->tree, settings);
    general = new general_class(this, shared, properties);

    main_menu = new main_menu_class(general, jobs, servers, groups, tasks);
    this->setMenuBar(main_menu);

    toolbar = new toolbar_class(this, general, jobs, tasks, servers, groups, log,
                                update, shared, settings, properties);
    toolbar->setMaximumHeight(40);

    // Layout
    splitter_top->addWidget(properties_parent);
    splitter_top->addWidget(add_title(jobs, "Jobs"));
    splitter_top->addWidget(tasks_parent);

    splitter_bottom->addWidget(servers_parents);
    splitter_bottom->addWidget(groups_parent);

    splitter_main->addWidget(toolbar);
    splitter_main->addWidget(splitter_top);
    splitter_main->addWidget(splitter_bottom);

    // Tamanio de Splitters
    splitter_main->setSizes({10, 100, 10});
    splitter_top->setSizes({120, 300, 120});
    splitter_bottom->setSizes({300, 120});
}

void monitor::closeEvent(QCloseEvent *event)
{
    if (shared->app_close)
        event->accept();
    else
    {
        event->ignore();
        this->hide();
    }
}
