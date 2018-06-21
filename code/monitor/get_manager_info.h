#ifndef GET_MANAGER_INFO
#define GET_MANAGER_INFO

// QT5 Library

#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QProgressBar>
#include <QLabel>
#include <QObject>
#include <QThread>
#include <QMainWindow>
#include <QTimer>
#include <QDebug>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>
//--------------------

#include <iostream>
using namespace std;
#include "../utils/tcp.h"
#include "../utils/threading.h"
#include "../utils/util.h"
#include "../utils/os.h"
#include "../manager/structs.h"
#include "../utils/json.h"
using namespace nlohmann;

// monitor moduless
#include "ui_jobs_tree.h"
#include "ui_servers_tree.h"
#include "ui_groups_tree.h"
#include "ui_tasks_tree.h"
#include "group_actions.h"
#include "shared_variables.h"

//-----------------

class get_manager_info : public QObject{
public:
    QMainWindow *monitor;

    ui_jobs_tree *jobsList;
    ui_servers_tree *serverList;
    ui_groups_tree *groupList;
    ui_tasks_tree *taskList;
    group_actions *groupActions;
    shared_variables *shared;

    template < class T >
    get_manager_info( T *_monitor ){
        monitor = _monitor;
        jobsList = _monitor->jobsList;
        serverList = _monitor->serverList;
        groupList = _monitor->groupList;
        taskList = _monitor->taskList;
        groupActions = _monitor->groupActions;
        shared = _monitor->shared;

        actions();
        managerRecieve();

    }

    bool task_first_add;
    vector <string> deleteList;
    QThread *getTask_thread = new QThread();

    // revice desde manager

    json managerRecieveUpdate( json recv );
    void actions();
    void managerRecieve();
    void updateJob( json recv );
    void updateServer( json recv );
    void updateGroup( json recv );
    void updateTask();
    void getTask();
    //-----------------------------

};

#endif //GET_MANAGER_INFO