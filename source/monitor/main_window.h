#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "includes.h"

#include "general.h"
#include "groups.h"
#include "jobs.h"
#include "log.h"
#include "main_menu.h"
#include "options.h"
#include "properties.h"
#include "servers.h"
#include "settings.h"
#include "tasks.h"
#include "toolbar.h"
#include "update.h"

#include <QSplitter>

#include <QTextEdit>
using namespace std;

class monitor : public QMainWindow
{
    Q_OBJECT

private:
    void closeEvent(QCloseEvent *event);

public:
    // Constructor y destructor
    monitor(QWidget *parent = 0);
    ~monitor();
    //---------------------------

    // Variables
    shared_variables *shared;
    // ----------------------------

    void setup_ui();
    QWidget *add_title(QWidget *widget, QString title);

    // Modulos
    main_menu_class *main_menu;
    jobs_class *jobs;
    servers_class *servers;
    tasks_class *tasks;
    groups_class *groups;
    update_class *update;
    general_class *global;
    settings_class *settings;
    toolbar_class *toolbar;
    log_class *log;
    options_class *options;
    properties_class *properties;
};

#endif // MAIN_WINDOW_H
