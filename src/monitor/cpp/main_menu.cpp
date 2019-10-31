#include "../hpp/main_menu.hpp"

main_menu_class::main_menu_class(
    Ui::MainWindow *_ui,
    global_class *_global)
{
    ui = _ui;
    global = _global;
    
    connections();
}

main_menu_class::~main_menu_class()
{
}

void main_menu_class::connections()
{
    // Main Menu-------------------------------------
    // QMenu *fileMenu = new QMenu("&File", this);
    // main_menu->addMenu(fileMenu);
    ui->file_menu->addAction(global->hide_action);
    ui->file_menu->addAction(global->quit_action);

    // QMenu *editMenu = new QMenu("&Edit", this);
    // mainMenu->addMenu(editMenu);
    ui->edit_menu->addAction(global->preferences_action);
    // ui->edit_menu->addAction(global->panelSubmitAction);
    // editMenu->addAction(_general->hidePanelsAction);

    // QMenu *serverMenu = new QMenu("&Servers", this);
    // mainMenu->addMenu(serverMenu);
    // serverMenu->addAction(serverActions->serverInactiveAction);
    // serverMenu->addAction(serverActions->serverReactiveAction);
    // serverMenu->addAction(serverActions->deleteAction);
    // serverMenu->addSeparator();
    // serverMenu->addAction(serverActions->serverMaxInstancesAction);
    // serverMenu->addSeparator();
    // serverMenu->addAction(serverActions->serverShowLog);
    // serverMenu->addAction(serverActions->serverFreeramAction);
    // serverMenu->addAction(serverActions->serverSshAction);

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
    // jobMenu->addAction(jobActions->jobLogAction);

    // QMenu *taskMenu = new QMenu("&Tasks", this);
    // mainMenu->addMenu(taskMenu);
    // taskMenu->addAction(taskActions->taskSuspendAction);
    // taskMenu->addSeparator();
    // taskMenu->addAction(taskActions->taskRestartAction);

    // QMenu *helpMenu = new QMenu("&Help", this);
    // mainMenu->addMenu(helpMenu);

    //--------------------------------------------------
}