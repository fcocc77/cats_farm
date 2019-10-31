#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include "includes.hpp"

class global_class : public QObject
{
private:
  shared_variables *shared;
  QMainWindow *monitor;
  Ui::MainWindow *ui;

  QSystemTrayIcon *notify;
  QStringList completed_jobs;

  // General Funcions
  void connections();
  void notify_icon();
  //----------------------------

public:
  global_class(
      Ui::MainWindow *_ui,
      QMainWindow *_monitor,
      shared_variables *_shared);

  // General Action
  QAction *preferences_action;
  QAction *quit_action;
  QAction *hide_action;
  QAction *show_action;
  QAction *hide_all_panels_action;
  //------------------------------------------------
};

#endif //GLOBAL_HPP