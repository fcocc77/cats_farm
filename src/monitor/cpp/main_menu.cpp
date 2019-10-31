#include "../hpp/main_menu.hpp"

main_menu_class::main_menu_class(
    Ui::MainWindow *_ui,
    global_class *_global,
    jobs_class *_jobs)
{
    ui = _ui;
    global = _global;
    jobs = _jobs;

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

    // serverMenu->addAction(serverActions->server_inactive_action);
    // serverMenu->addAction(serverActions->server_reactive_action);
    // serverMenu->addAction(serverActions->deleteAction);
    // serverMenu->addSeparator();
    // serverMenu->addAction(serverActions->server_max_instances_action);
    // serverMenu->addSeparator();
    // serverMenu->addAction(serverActions->server_show_log);
    // serverMenu->addAction(serverActions->server_free_ram_action);
    // serverMenu->addAction(serverActions->server_ssh_action);

    // QMenu *groupMenu = new QMenu("&Groups", this);
    // mainMenu->addMenu(groupMenu);
    // groupMenu->addAction(groupActions->groupCreateAction);
    // groupMenu->addAction(groupActions->deleteAction);
    // groupMenu->addSeparator();
    // groupMenu->addAction(groupActions->groupAddmachineAction);

    // QMenu *jobMenu = new QMenu("&Jobs", this);
    // mainMenu->addMenu(jobMenu);
    // jobMenu->addAction(jobActions->jobResumeAction);
    // jobMenu->addAction(jobActions->jobSuspendAction);
    // jobMenu->addAction(jobActions->deleteAction);
    // jobMenu->addAction(jobActions->jobRestartAction);
    // jobMenu->addSeparator();
    // jobMenu->addAction(jobActions->jobShowRenderFolderAction);
    // jobMenu->addSeparator();
    // jobMenu->addAction(jobActions->jobModifyAction);
    ui->jobs_menu->addAction(jobs->job_log_action);

    // QMenu *taskMenu = new QMenu("&Tasks", this);
    // mainMenu->addMenu(taskMenu);
    // taskMenu->addAction(taskActions->taskSuspendAction);
    // taskMenu->addSeparator();
    // taskMenu->addAction(taskActions->taskRestartAction);

    // QMenu *helpMenu = new QMenu("&Help", this);
    // mainMenu->addMenu(helpMenu);

    //--------------------------------------------------
}