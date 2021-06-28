#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

#include "shared_variables.h"
#include "log.h"
#include "options.h"
#include "settings.h"
#include "properties.h"
#include "servers.h"
#include "groups.h"
#include "jobs.h"
#include "tasks.h"
#include "update.h"
#include "general.h"
#include "main_menu.h"
#include "toolbar.h"
#include "submit.h"


using namespace std;

class monitor : public QMainWindow
{
    Q_OBJECT

private:
    shared_variables *shared;

    void setup_ui();
    QWidget *add_title(QWidget *widget, QString title);

    log_class *log;
    options_class *options;
    settings_class *settings;
    properties_class *properties;

    servers_class *servers;
    groups_class *groups;
    jobs_class *jobs;
    tasks_class *tasks;

    update_class *update;
    general_class *general;
    main_menu_class *main_menu;
    toolbar_class *toolbar;

    submit *_submit;

    void closeEvent(QCloseEvent *event) override;

public:
    monitor(QWidget *parent = 0);
    ~monitor();

    inline QWidget *get_groups_widget() const;
    inline toolbar_class *get_toolbar() const;
};

inline toolbar_class *monitor::get_toolbar() const
{
    return toolbar;
}

inline QWidget *monitor::get_groups_widget() const
{
    return groups;
}

#endif // MAIN_WINDOW_H
