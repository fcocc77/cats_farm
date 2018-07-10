#ifndef UI_TASKS_TREE
#define UI_TASKS_TREE

// QT5 Library
#include <QTreeWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QStringList>
#include <QDockWidget>
#include <QMenuBar>
//-------------

#include <iostream>
using namespace std;

class ui_tasks_tree : public QTreeWidget {
public:
    ui_tasks_tree();

    void ui();

	QDockWidget *dock = new QDockWidget("Tasks");

};

#endif //UI_TASKS_TREE