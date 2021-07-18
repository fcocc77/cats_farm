#include <QTextEdit>

#include "../global/global.h"
#include "main_window.h"
#include "util.h"

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

    toolbar = new toolbar_class(this, general, jobs, tasks, servers, groups,
                                log, update, shared, settings, properties);
    toolbar->setMaximumHeight(40);

    _layout_widget = new layout_widget(properties, jobs, tasks->get_widget(),
                                       groups, servers, toolbar);

    QWidget *central_widget = new QWidget;
    QVBoxLayout *central_layout = new QVBoxLayout(central_widget);
    central_layout->setMargin(0);
    central_layout->setSpacing(0);

    central_layout->addWidget(toolbar);
    central_layout->addWidget(_layout_widget);

    this->setCentralWidget(central_widget);
}

void monitor::closeEvent(QCloseEvent *event)
{
    _layout_widget->save_layout();

    if (shared->app_close)
        event->accept();
    else
    {
        event->ignore();
        this->hide();
    }
}
