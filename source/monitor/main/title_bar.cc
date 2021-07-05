#include <QHBoxLayout>

#include "title_bar.h"

title_bar::title_bar(QString label)
{
    this->setObjectName("title_bar");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(3);


    title_label = new QLabel(label);

    layout->addWidget(title_label);
}
