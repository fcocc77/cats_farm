// Author: Francisco Contreras
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#ifndef LAYOUT_H
#define LAYOUT_H

#include <QSplitter>
#include <QWidget>

#include "toolbar.h"

class layout_widget : public QWidget
{
private:
    void restore_layout();
    QWidget *properties, *jobs, *tasks, *groups, *servers;

    toolbar_class *toolbar;

public:
    layout_widget(QWidget *properties, QWidget *jobs, QWidget *tasks,
                  QWidget *groups, QWidget *servers, toolbar_class *toolbar);

    void save_layout();
};

#endif // LAYOUT_H
