#include <QVBoxLayout>

#include "util.h"
#include <combo_box.h>

combo_box::combo_box()
    : menu(new QMenu(this))
    , label(new QLabel)
    , index(0)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    this->setObjectName("combo_box");

    connect(menu, &QMenu::triggered, this,
            [=](QAction *action) { set_current_text(action->text()); });

    // Layout
    layout->addWidget(label);
}

combo_box::~combo_box() {}

void combo_box::add_item(QString item)
{
    QAction *action = new QAction(item);
    menu->addAction(action);

    actions.push_back(action);
}

void combo_box::add_items(QStringList items)
{
    for (QString item : items)
        add_item(item);
}

void combo_box::set_current_index(int _index)
{
    if (_index == -1)
        return;

    index = _index;

    QString text = actions[index]->text();
    label->setText(text);

    current_text_changed(text);
}

void combo_box::set_current_text(QString text)
{
    set_current_index(get_index(text));
}

int combo_box::get_index(QString text) const
{
    for (int i = 0; i < actions.count(); i++)
    {
        QAction *action = actions[i];
        if (action->text() == text)
            return i;
    }

    return -1;
}

void combo_box::clear()
{
    for (QAction *action : actions)
        delete action;

    menu->clear();
    actions.clear();
}

QString combo_box::get_current_text() const
{
    return label->text();
}

void combo_box::mousePressEvent(QMouseEvent *event)
{
    clicked();

    menu->popup(this->mapToGlobal({0, this->height()}));
    menu->show();

    QWidget::mousePressEvent(event);
}
