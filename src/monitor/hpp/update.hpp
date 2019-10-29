#ifndef UPDATE_HPP
#define UPDATE_HPP

#include "../hpp/includes.hpp"

class update_class : public QObject
{
private:
  Ui::MainWindow *ui;

  // variables
  shared_variables *shared;
  QTreeWidget *jobs;
  QTreeWidget *servers;
  QTreeWidget *tasks_tree;
  QTreeWidget *groups;
  QStringList deleteList;
  // group_actions *groupActions;
  // --------------------

  // Tasks
  bool task_first_add;
  QThread *get_task_thread;
  // ----------------------

  // funciones
  void manager_recieve();
  QString manager_recieve_update(QString recv, QJsonObject extra);
  void update_jobs(QJsonObject recv);
  void update_servers(QJsonObject recv);
  void update_groups(QJsonObject recv);
  void get_task();
  void update_task();
  void connections();
  // ----------------------

public:
  update_class(Ui::MainWindow *_ui, shared_variables *_shared);
};

#endif //UPDATE_HPP