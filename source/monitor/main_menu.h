#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "global.h"
#include "groups.h"
#include "includes.h"
#include "jobs.h"
#include "servers.h"
#include "tasks.h"

class main_menu_class : public QMenuBar
{
private:
    void connections();

    global_class *global;
    jobs_class *jobs;
    servers_class *servers;
    groups_class *groups;
    tasks_class *tasks;

public:
    main_menu_class(global_class *_global, jobs_class *_jobs,
                    servers_class *_servers, groups_class *_groups,
                    tasks_class *_tasks);
    ~main_menu_class();
};

#endif // MAIN_MENU_HPP
