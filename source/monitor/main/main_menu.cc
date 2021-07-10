#include <QMenu>

#include "main_menu.h"

main_menu_class::main_menu_class(general_class *_global, jobs_class *_jobs,
                                 servers_class *_servers, groups_class *_groups,
                                 tasks_class *_tasks)

    : global(_global)
    , jobs(_jobs)
    , servers(_servers)
    , groups(_groups)
    , tasks(_tasks)
{
    connections();
}

main_menu_class::~main_menu_class() {}

void main_menu_class::connections()
{
    QMenu *file_menu = new QMenu("File", this);
    this->addMenu(file_menu);
    file_menu->addAction(global->hide_action);
    file_menu->addAction(global->quit_action);

    QMenu *edit_menu = new QMenu("Edit", this);
    this->addMenu(edit_menu);
    edit_menu->addAction(global->settings_action);
    edit_menu->addAction(global->properties_hide);

    QMenu *servers_menu = new QMenu("Servers", this);
    this->addMenu(servers_menu);
    servers_menu->addAction(servers->server_inactive_action);
    servers_menu->addAction(servers->server_reactive_action);
    servers_menu->addAction(servers->delete_action);
    servers_menu->addSeparator();
    servers_menu->addAction(servers->server_free_ram_action);

    QMenu *groups_menu = new QMenu("Groups", this);
    this->addMenu(groups_menu);
    groups_menu->addAction(groups->create_action);
    groups_menu->addAction(groups->delete_action);
    groups_menu->addSeparator();
    groups_menu->addAction(groups->add_machine_action);

    QMenu *jobs_menu = new QMenu("Jobs", this);
    this->addMenu(jobs_menu);
    jobs_menu->addAction(jobs->job_resume_action);
    jobs_menu->addAction(jobs->job_pause_action);
    jobs_menu->addAction(jobs->delete_action);
    jobs_menu->addAction(jobs->job_restart_action);
    jobs_menu->addSeparator();
    jobs_menu->addAction(jobs->job_modify_action);
    jobs_menu->addAction(jobs->job_log_action);

    QMenu *tasks_menu = new QMenu("Tasks", this);
    this->addMenu(tasks_menu);
    tasks_menu->addAction(tasks->pause_action);
    tasks_menu->addSeparator();
    tasks_menu->addAction(tasks->restart_action);

    QMenu *extra_menu = new QMenu("Extra", this);
    this->addMenu(extra_menu);
    extra_menu->addAction(global->update_style_action);
}
