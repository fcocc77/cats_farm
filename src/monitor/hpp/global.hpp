#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include "includes.hpp"

// Monitor Library
// #include "ui_jobs_tree.h"
// #include "settings.h"
// #include "ui_submit.h"
//----------

class global_class : public QObject
{
private:
  shared_variables *shared;
  QMainWindow *monitor;
  Ui::MainWindow *ui;

public:
  // settings *_settings;

  // ui_submit *uiSubmit;
  // QDockWidget *uiJobOptions;
  // QDockWidget *uiServerOptions;
  // QDockWidget *log_dock;

  // ui_jobs_tree *jobsList;
  // shared_variables *shared;
  // QStringList completedJobs;

  global_class(
      Ui::MainWindow *_ui,
      QMainWindow *_monitor,
      shared_variables *_shared);

  QSystemTrayIcon *notify;

  // General Action
  QAction *preferences_action = new QAction("Preferences");
  QAction *quit_action = new QAction("Quit");
  QAction *hide_action = new QAction("Hide");
  QAction *show_action = new QAction("Show App");
  QAction *panelSubmitAction = new QAction("Submit");
  QAction *hidePanelsAction = new QAction("Hide Panels");
  //------------------------------------------------

  // General Funcions
  void init();
  void connections();
  void notifyIcon();
  void style_ui();
  void show_splash();
  //----------------------------
};

#endif //GLOBAL_HPP