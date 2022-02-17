// Author: Francisco Jose Contreras Cuevas
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#include <QHBoxLayout>

#include "../global/global.h"
#include "util.h"
#include <combo_box.h>

combo_box::combo_box()
    : menu(new QMenu(this))
    , label(new QLabel)
    , index(0)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    this->setObjectName("combo_box");
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    this->setFixedHeight(23);
    this->setMinimumWidth(70);

    arrow = new QLabel;
    arrow->setFixedWidth(22);
    arrow->setObjectName("arrow");
    update_arrow("normal");

    label->setObjectName("combobox_label");

    connect(menu, &QMenu::triggered, this,
            [=](QAction *action) { set_current_text(action->text(), true); });

    // Layout
    layout->addWidget(label);
    layout->addWidget(arrow);
}

combo_box::~combo_box() {}

void combo_box::update_arrow(QString state)
{
    QPixmap pixmap(VINARENDER_PATH + "/resources/images/arrow_down_" + state +
                   ".png");

    arrow->setPixmap(
        pixmap.scaled(22, 22, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

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

void combo_box::set_index(int _index, bool emmit_signal)
{
    if (_index == -1)
        return;

    index = _index;

    QString text = actions[index]->text();
    label->setText(text);

    if (emmit_signal)
    {
        text_changed(text);
        index_changed(index);
    }
}

void combo_box::set_current_text(QString text, bool emmit_signal)
{
    set_index(get_index(text), emmit_signal);
}

int combo_box::get_index(QString text) const
{
    for (int i = 0; i < actions.count(); i++)
    {
        QAction *action = actions[i];
        if (action->text().toLower() == text.toLower())
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

void combo_box::mousePressEvent(QMouseEvent *event)
{
    clicked();

    menu->popup(this->mapToGlobal({0, this->height()}));
    menu->show();

    QWidget::mousePressEvent(event);
}

void combo_box::set_disabled(bool disable)
{
    this->setDisabled(disable);
    disable ? update_arrow("disable") : update_arrow("normal");
}
