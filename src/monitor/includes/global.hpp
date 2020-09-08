#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include "includes.hpp"

class global_class : public QObject
{
private:
  shared_variables *shared;
  QMainWindow *monitor;

  QSystemTrayIcon *notify;
  QStringList completed_jobs;
  QDockWidget *settings_dock;
  QDockWidget *options_dock;
  QDockWidget *log_dock;

  // General Funcions
  void setup_ui();
  void connections();
  void notify_icon();
  void style();
  //----------------------------

public:
  global_class(
      QMainWindow *_monitor,
      shared_variables *_shared,
      QDockWidget *_settings_dock,
      QDockWidget *_options_dock,
      QDockWidget *_log_dock);

  // General Action
  QAction *preferences_action;
  QAction *quit_action;
  QAction *hide_action;
  QAction *show_action;
  QAction *hide_all_panels_action;
  QAction *update_style_action;
  //------------------------------------------------
};

#endif //GLOBAL_HPP