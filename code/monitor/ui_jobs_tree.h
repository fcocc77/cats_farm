#ifndef UI_JOBS_TREE
#define UI_JOBS_TREE

// QT5 Library
#include <QTreeWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QStringList>
#include <QDropEvent>
#include <QString>
#include <QDebug>
#include <QList>
#include <QUrl>
#include <QMimeData>

//-------------

#include "ui_submit.h"
#include "ui_tasks_tree.h"

using namespace std;

class ui_jobs_tree : public QTreeWidget {
Q_OBJECT
public:

    ui_submit *uiSubmit;
    ui_tasks_tree *taskList;

    template < class T >
    ui_jobs_tree( T *_monitor ){
        uiSubmit = _monitor->uiSubmit;
        taskList = _monitor->taskList;
        ui();
        this->setAcceptDrops( true );

    }

    void ui();
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);

    QWidget *widget = new QWidget();

};

#endif //UI_JOBS_TREE