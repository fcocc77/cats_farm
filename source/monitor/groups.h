#ifndef GROUPS_HPP
#define GROUPS_HPP

#include "includes.h"
#include "servers.h"

class groups_class : public QTreeWidget
{
private:
    QMainWindow *monitor;
    shared_variables *shared;
    servers_class *servers;

    // Groups Funtions
    void setup_ui();
    void connections();
    void popup();
    void create_window();
    void add_machine();
    void group_delete();
    //-----------------

public:
    groups_class(QMainWindow *_monitor, shared_variables *_shared,
                 servers_class *_servers);
    ~groups_class();

    // Group Action
    QAction *create_action;
    QAction *add_machine_action;
    QAction *delete_action;
    //------------------------------------------------

    void make_server(QTreeWidgetItem *item, QJsonArray machines);
    QTreeWidgetItem *group_make(QString group_name, int totaMachine,
                                int activeMachine, int offMachine);
};

class ElidedLabel : public QLabel
{
public:
    QString cachedElidedText;

    void paintEvent(QPaintEvent *e)
    {
        QPainter p(this);
        p.drawText(0, 0, geometry().width(), geometry().height(), alignment(),
                   cachedElidedText);
    }

    void resizeEvent(QResizeEvent *e)
    {
        QLabel::resizeEvent(e);
        cacheElidedText(e->size().width());
    }

    void cacheElidedText(int w)
    {
        cachedElidedText = fontMetrics().elidedText(text(), Qt::ElideRight, w,
                                                    Qt::TextShowMnemonic);
    }
};

#endif // GROUPS_HPP
