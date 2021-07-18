#include <QVBoxLayout>

#include "layout_widget.h"

layout_widget::layout_widget(QWidget *properties, QWidget *jobs, QWidget *tasks,
                             QWidget *groups, QWidget *servers)
{

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);

    // Splitter
    QSplitter *splitter_main = new QSplitter(this);
    splitter_main->setOrientation(Qt::Vertical);

    QSplitter *splitter_top = new QSplitter(this);
    QSplitter *splitter_bottom = new QSplitter(this);

    // Layout
    splitter_top->addWidget(properties);
    splitter_top->addWidget(jobs);
    splitter_top->addWidget(tasks);

    splitter_bottom->addWidget(servers);
    splitter_bottom->addWidget(groups);

    splitter_main->addWidget(splitter_top);
    splitter_main->addWidget(splitter_bottom);

    // Tamanio de Splitters
    splitter_main->setSizes({10, 100, 10});
    splitter_top->setSizes({120, 300, 120});
    splitter_bottom->setSizes({300, 120});

    layout->addWidget(splitter_main);
}
