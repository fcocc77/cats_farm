#ifndef GROUPS_HPP
#define GROUPS_HPP

#include <QMainWindow>
#include <QPainter>
#include <QResizeEvent>
#include <QTreeWidget>
#include <QVBoxLayout>

#include "servers_widget.h"
#include "title_bar.h"

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

class groups_class : public QWidget
{
private:
    QWidget *_monitor;
    shared_variables *shared;
    servers_class *servers;

    QVBoxLayout *layout;
    QTreeWidget *tree;
    title_bar *_title_bar;

    // Groups Funtions
    void setup_ui();
    void connections();
    void popup();
    void create_window();
    void add_machine();
    void group_delete();

public:
    groups_class(QWidget *_monitor, shared_variables *_shared,
                 servers_class *_servers);
    ~groups_class();

    // Group Action
    QAction *create_action;
    QAction *add_machine_action;
    QAction *delete_action;

    void make_server(QTreeWidgetItem *item, QJsonArray machines);
    QTreeWidgetItem *group_make(QString group_name, int totaMachine,
                                int activeMachine, int offMachine);

    QStringList get_groups() const;
    inline QTreeWidget *get_tree() const;
};

inline QTreeWidget *groups_class::get_tree() const
{
    return tree;
}


#endif // GROUPS_HPP
