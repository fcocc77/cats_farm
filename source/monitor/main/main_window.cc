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
    QString openMonitor = VINARENDER_CONF_PATH + "/openMonitor";

    // guarda un bool para ver si el monitor ya esta cerrado.
    fwrite(openMonitor, "0");
}

void monitor::setup_ui()
{
    shared = new shared_variables();
    shared->settings = jread(VINARENDER_CONF_PATH + "/settings.json");

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

    log = new log_class();
    options = new options_class(this, shared);
    settings = new settings_class(this, shared);
    _submit = new submit(this);

    properties = new properties_class(log, options, settings, _submit);
    servers = new servers_class(this, shared, log);
    groups = new groups_class(this, shared, servers);

    jobs = new jobs_class(shared, this, log, servers, options, groups,
                          properties, _submit);

    tasks = new tasks_class(this, shared, jobs);

    update = new update_class(shared, groups, jobs, servers, tasks->get_tree(),
                              settings);

    general = new general_class(this, shared, properties);

    main_menu = new main_menu_class(general, jobs, servers, groups, tasks);
    this->setMenuBar(main_menu);

    toolbar = new toolbar_class(this, general, jobs, tasks, servers, groups, log,
                                update, shared, settings, properties);
    toolbar->setMaximumHeight(40);

    // Layout
    splitter_top->addWidget(properties);
    splitter_top->addWidget(jobs);
    splitter_top->addWidget(tasks->get_widget());

    splitter_bottom->addWidget(servers);
    splitter_bottom->addWidget(groups);

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
