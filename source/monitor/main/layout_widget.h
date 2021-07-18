#ifndef LAYOUT_H
#define LAYOUT_H

#include <QWidget>
#include <QSplitter>

class layout_widget : public QWidget
{

public:
    layout_widget(QWidget *properties, QWidget *jobs, QWidget *tasks, QWidget *groups,
           QWidget *servers);
};

#endif // LAYOUT_H
