#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "../hpp/includes.hpp"

#include <iostream>
using namespace std;

#include "main_menu.hpp"
#include "jobs.hpp"
#include "servers.hpp"
#include "groups.hpp"
#include "tasks.hpp"
#include "update.hpp"
#include "global.hpp"
#include "settings.hpp"
#include "toolbar.hpp"
#include "log.hpp"
#include "options.hpp"

// monitor modules
// #include "ui_submit.h"
// #include "ui_jobs_tree.h"
// #include "ui_servers_tree.h"
// #include "ui_groups_tree.h"
// #include "ui_tasks_tree.h"
// #include "ui_job_options.h"
// #include "ui_server_options.h"
// #include "get_manager_info.h"
// #include "group_actions.h"
// #include "jobs_actions.h"
// #include "general.h"
// #include "servers_actions.h"
// #include "tasks_actions.h"
// #include "shared_variables.h"
// #include "../external/QCodeEditor/include/KGL/Widgets/QCodeEditor.hpp"
//--------------------------------------
#include <QTextEdit>
using namespace std;

class monitor : public QMainWindow
{
  Q_OBJECT

private:
  // void closeEvent(QCloseEvent *event);

public:
  // Constructor y destructor
  monitor(QWidget *parent = 0);
  ~monitor();
  //---------------------------

  Ui::MainWindow *ui;

  // Variables
  shared_variables *shared;
  // ----------------------------

  // Modulos
  main_menu_class *menu_bar;
  jobs_class *jobs;
  servers_class *servers;
  tasks_class *tasks;
  groups_class *groups;
  update_class *update;
  global_class *global;
  settings_class *settings;
  toolbar_class *toolbar;
  log_class *log;
  options_class *options;

  // --------------------------------

  // QToolBar *toolBar = new QToolBar();

  //log


  // declaracion de modulos
  // ui_servers_tree *serverList = new ui_servers_tree(this);
  // ui_groups_tree *groupList = new ui_groups_tree();
  // ui_submit *uiSubmit = new ui_submit(this);
  // ui_tasks_tree *taskList = new ui_tasks_tree();
  // ui_jobs_tree *jobsList = new ui_jobs_tree(this);
  // ui_server_options *uiServerOptions = new ui_server_options();
  // ui_job_options *uiJobOptions = new ui_job_options();
  // group_actions *groupActions = new group_actions(this);
  // jobs_actions *jobActions = new jobs_actions(this);
  // servers_actions *serverActions = new servers_actions(this);
  // tasks_actions *taskActions = new tasks_actions(this);
  // general *_general = new general(this);
  //------------------------------------------------------------------------

  // void init();
  // void assamble();
  // void update();
  // void main_menu();
  // void tool_bar();
  // void log_ui();
};

#endif //MAIN_WINDOW_H