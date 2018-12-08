#ifndef UI_SERVER_TREE
#define UI_SERVER_TREE

// QT5 Library
#include <QTreeWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QStringList>
#include <QDockWidget>
#include <QMenuBar>
#include <QMainWindow>
#include <QDebug>
//-------------
using namespace std;
#include <iostream>

#include "shared_variables.h"

class ui_servers_tree : public QTreeWidget
{
  public:
    QMainWindow *monitor;
    shared_variables *shared;

    template <class T>
    ui_servers_tree(T *_monitor)
    {
        monitor = _monitor;
        shared = _monitor->shared;
        ui();
        actions();
    }

    void ui();
    void actions();

    QDockWidget *dock = new QDockWidget("Servers");

    // display job list
    QAction *showAllAction = new QAction("Show All", this);
    QAction *hideAllAction = new QAction("Hide All", this);
    QAction *displayWindowsAction = new QAction("Windows", this);
    QAction *displayLinuxAction = new QAction("Linux", this);
    QAction *displayMacAction = new QAction("Mac OS X", this);
    QAction *displayOnAction = new QAction("ON", this);
    QAction *displayOffAction = new QAction("OFF", this);
    //------------------------------------------------
};

#endif //UI_SERVER_TREE