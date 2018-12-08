#ifndef GROUP_ACTIONS_H
#define GROUP_ACTIONS_H

// QT5 Library
#include <QMenu>
#include <QTreeWidgetItem>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QDir>
#include <QLabel>
#include <QPainter>
#include <QFontMetrics>
#include <QTreeWidget>
#include <QObject>
#include <QMainWindow>
#include <QResizeEvent>
//--------------

#include "../utils/tcp.h"
#include "../utils/util.h"

using namespace std;

// monitor modules
#include "ui_servers_tree.h"
#include "ui_groups_tree.h"
#include "shared_variables.h"

//-----------------

class group_actions : public QObject
{
  public:
    QMainWindow *monitor;
    ui_servers_tree *serverList;
    ui_groups_tree *groupList;
    shared_variables *shared;

    template <class T>
    group_actions(T *_monitor)
    {
        monitor = _monitor;
        serverList = _monitor->serverList;
        groupList = _monitor->groupList;
        shared = _monitor->shared;

        actions();
    }

    // variables Generales
    const QString managerHost = fread("../../etc/manager_host");
    //--------------------------------------

    // Group Action
    QAction *groupCreateAction = new QAction("Create Group");
    QAction *groupAddmachineAction = new QAction("Add Machine");
    QAction *deleteAction = new QAction("Delete Group");

    //------------------------------------------------

    // Groups Funtions
    void actions();
    void group_popup();
    void groupCreateWindow();
    QTreeWidgetItem *groupMake(QString group_name, int totaMachine, int activeMachine, int offMachine);
    void groupMakeServer(QTreeWidgetItem *item, QStringList machines);
    void groupAddMachine();
    void groupDelete();
    //-----------------
};

class ElidedLabel : public QLabel
{
  public:
    QString cachedElidedText;

    void paintEvent(QPaintEvent *e)
    {
        QPainter p(this);
        p.drawText(0, 0, geometry().width(), geometry().height(), alignment(), cachedElidedText);
    }

    void resizeEvent(QResizeEvent *e)
    {
        QLabel::resizeEvent(e);
        cacheElidedText(e->size().width());
    }

    void cacheElidedText(int w)
    {
        cachedElidedText = fontMetrics().elidedText(text(), Qt::ElideRight, w, Qt::TextShowMnemonic);
    }
};

#endif //GROUP_ACTIONS_H