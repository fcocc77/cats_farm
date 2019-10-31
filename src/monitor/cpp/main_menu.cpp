#include "../hpp/main_menu.hpp"

main_menu_class::main_menu_class(
    Ui::MainWindow *_ui,
    global_class *_global,
    jobs_class *_jobs,
    servers_class *_servers,
    groups_class *_groups,
    tasks_class *_tasks)
{
    ui = _ui;
    global = _global;
    jobs = _jobs;
    servers = _servers;
    groups = _groups;
    tasks = _tasks;

    connections();
}

main_menu_class::~main_menu_class()
{
}

void main_menu_class::connections()
{
    ui->file_menu->addAction(global->hide_action);
    ui->file_menu->addAction(global->quit_action);
    ui->edit_menu->addAction(global->preferences_action);
    ui->edit_menu->addAction(global->hide_all_panels_action);

    ui->servers_menu->addAction(servers->server_inactive_action);
    ui->servers_menu->addAction(servers->server_reactive_action);
    ui->servers_menu->addAction(servers->delete_action);
    ui->servers_menu->addSeparator();
    ui->servers_menu->addAction(servers->server_max_instances_action);
    ui->servers_menu->addSeparator();
    ui->servers_menu->addAction(servers->server_show_log);
    ui->servers_menu->addAction(servers->server_free_ram_action);
    ui->servers_menu->addAction(servers->server_ssh_action);

    ui->groups_menu->addAction(groups->create_action);
    ui->groups_menu->addAction(groups->delete_action);
    ui->groups_menu->addSeparator();
    ui->groups_menu->addAction(groups->add_machine_action);

    ui->jobs_menu->addAction(jobs->job_resume_action);
    ui->jobs_menu->addAction(jobs->job_suspend_action);
    ui->jobs_menu->addAction(jobs->delete_action);
    ui->jobs_menu->addAction(jobs->job_restart_action);
    ui->jobs_menu->addSeparator();
    ui->jobs_menu->addAction(jobs->job_modify_action);
    ui->jobs_menu->addAction(jobs->job_log_action);

    ui->tasks_menu->addAction(tasks->suspend_action);
    ui->tasks_menu->addSeparator();
    ui->tasks_menu->addAction(tasks->restart_action);

    ui->extra_menu->addAction(global->update_style_action);
}