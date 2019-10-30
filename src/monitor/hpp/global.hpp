#ifndef GENERAL_H
#define GENERAL_H

#include "../hpp/includes.hpp"

// Monitor Library
// #include "ui_jobs_tree.h"
// #include "settings.h"
// #include "ui_submit.h"
//----------



class global_class : public QObject
{
private:
 

public:
  // settings *_settings;

  // ui_submit *uiSubmit;
  // QDockWidget *uiJobOptions;
  // QDockWidget *uiServerOptions;
  // QDockWidget *log_dock;

  // QMainWindow *monitor;
  // ui_jobs_tree *jobsList;
  // shared_variables *shared;
  // QStringList completedJobs;

  global_class(Ui::MainWindow *ui)
  {
    // _settings = new settings(_monitor);
    // monitor = _monitor;
    // uiSubmit = _monitor->uiSubmit;
    // uiJobOptions = _monitor->uiJobOptions;
    // uiServerOptions = _monitor->uiServerOptions;
    // log_dock = _monitor->log_dock;
    // jobsList = _monitor->jobsList;
    // shared = _monitor->shared;

    init();
  }

  QSystemTrayIcon *notify;

  // General Action
  QAction *preferences_action = new QAction("Preferences");
  QAction *quitAction = new QAction("Quit");
  QAction *hideAction = new QAction("Hide");
  QAction *showAction = new QAction("Show App");
  QAction *aboutAction = new QAction("About");
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

#endif //GENERAL_H