#ifndef UI_GROUPS_TREE
#define UI_GROUPS_TREE

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

class ui_groups_tree : public QTreeWidget {
public:
    ui_groups_tree();

    void ui();

	QDockWidget *dock = new QDockWidget("Groups");

};

#endif //UI_GROUPS_TREE