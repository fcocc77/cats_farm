// Author: Francisco Contreras
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include <QMenuBar>

#include "general.h"
#include "groups_widget.h"
#include "servers_widget.h"
#include "tasks_widget.h"
#include "jobs_widget.h"

class main_menu_class : public QMenuBar
{
private:
    void connections();

    general_class *global;
    jobs_class *jobs;
    servers_class *servers;
    groups_class *groups;
    tasks_class *tasks;

public:
    main_menu_class(general_class *_global, jobs_class *_jobs,
                    servers_class *_servers, groups_class *_groups,
                    tasks_class *_tasks);
    ~main_menu_class();
};

#endif // MAIN_MENU_HPP
