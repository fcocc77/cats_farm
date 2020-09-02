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
  QWidget *options_widget;
  QWidget *log_widget;
  QWidget *properties;

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
      QWidget *_options_widget,
      QWidget *_log_widget,
      QWidget *_properties);

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