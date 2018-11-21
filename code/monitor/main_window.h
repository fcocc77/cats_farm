#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QAction>

// QT5 Library

#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QPlainTextEdit>
#include <QDockWidget>
#include <QVBoxLayout>
//------------------------

#include <iostream>
using namespace std;

// monitor modules
#include "ui_submit.h"
#include "ui_jobs_tree.h"
#include "ui_servers_tree.h"
#include "ui_groups_tree.h"
#include "ui_tasks_tree.h"
#include "ui_job_options.h"
#include "ui_server_options.h"
#include "ui_denoiser.h"
#include "get_manager_info.h"
#include "group_actions.h"
#include "jobs_actions.h"
#include "general.h"
#include "servers_actions.h"
#include "tasks_actions.h"
#include "shared_variables.h"
#include "../external/QCodeEditor/include/KGL/Widgets/QCodeEditor.hpp"
//--------------------------------------
#include <QTextEdit>
using namespace std;

class monitor : public QMainWindow{
Q_OBJECT

public:
    // este es el constructor
    monitor(){QMainWindow(0);}; 
    //---------------------------

    ~monitor(){
        QString openMonitor = path + "/etc/openMonitor";
        fwrite( openMonitor, "0" ); // escrebe que el monitor ya esta cerrado
    }

    bool app_started = false;
    QToolBar *toolBar = new QToolBar();

    //log
    kgl::QCodeEditor *log_text = new kgl::QCodeEditor(this);
    QDockWidget *log_dock = new QDockWidget( "Logs" );

    shared_variables *shared = new shared_variables();

    // declaracion de modulos
    ui_servers_tree *serverList = new ui_servers_tree( this );
    ui_groups_tree *groupList = new ui_groups_tree();
    ui_submit *uiSubmit = new ui_submit( this );
    ui_denoiser *uiDenoiser = new ui_denoiser( this );
    ui_tasks_tree *taskList = new ui_tasks_tree();
    ui_jobs_tree *jobsList = new ui_jobs_tree( this );
    ui_server_options *uiServerOptions = new ui_server_options();
    ui_job_options *uiJobOptions = new ui_job_options();
    group_actions *groupActions = new group_actions( this );
    get_manager_info *getManagerInfo = new get_manager_info( this );
    jobs_actions *jobActions = new jobs_actions( this );
    servers_actions *serverActions = new servers_actions( this );
    tasks_actions *taskActions = new tasks_actions( this );
    general *_general = new general( this );
    //------------------------------------------------------------------------

    void init();
    void assamble();
    //void ui();
    void update();
    void main_menu();
    void tool_bar();
    void log_ui();

};

#endif //MAIN_WINDOW_H