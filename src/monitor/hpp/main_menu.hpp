#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "includes.hpp"
#include "global.hpp"
#include "jobs.hpp"
#include "servers.hpp"
#include "groups.hpp"
#include "tasks.hpp"

class main_menu_class
{
private:
    Ui::MainWindow *ui;
    void connections();

    global_class *global;
    jobs_class *jobs;
    servers_class *servers;
    groups_class *groups;
    tasks_class *tasks;

public:
    main_menu_class(
        Ui::MainWindow *_ui,
        global_class *_global,
        jobs_class *_jobs,
        servers_class *_servers,
        groups_class *_groups,
        tasks_class *_tasks);
    ~main_menu_class();
};

#endif //MAIN_MENU_HPP