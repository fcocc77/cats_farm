#ifndef UPDATE_HPP
#define UPDATE_HPP

#include "includes.hpp"
#include "groups.hpp"
#include "servers.hpp"
#include "jobs.hpp"
#include "settings.hpp"
#include <progress_bar.hpp>

class update_class : public QObject
{
private:
  // variables
  shared_variables *shared;
  groups_class *groups;
  jobs_class *jobs;
  servers_class *servers;
  QTreeWidget *tasks;
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
      shared_variables *_shared,
      groups_class *_groups,
      jobs_class *_jobs,
      servers_class *_servers,
      QTreeWidget *_tasks_tree,
      settings_class *_settings);

  void update(QString host);
};

#endif //UPDATE_HPP