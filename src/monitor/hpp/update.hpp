#ifndef UPDATE_HPP
#define UPDATE_HPP

#include "includes.hpp"
#include "groups.hpp"
#include "settings.hpp"

class update_class : public QObject
{
private:
  Ui::MainWindow *ui;

  // variables
  shared_variables *shared;
  groups_class *groups;
  QStringList delete_list;
  tcp_client_widget<update_class> *manager;
  settings_class *settings;
  bool update_started = false;
  // --------------------

  // Tasks
  bool task_first_add;
  QThread *get_task_thread;
  // ----------------------

  // funciones
  QString manager_recieve_update(QString recv, QJsonObject extra);
  void update_jobs(QJsonObject recv);
  void update_servers(QJsonObject recv);
  void update_groups(QJsonObject recv);
  void get_task();
  void update_task();
  void connections();
  // ----------------------

public:
  update_class(
      Ui::MainWindow *_ui,
      shared_variables *_shared,
      groups_class *_groups,
      settings_class *_settings);

  void update(QString host);
};

#endif //UPDATE_HPP