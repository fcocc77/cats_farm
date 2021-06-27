#include <combo_box.h>

combo_box::combo_box() {}

combo_box::~combo_box() {}

void combo_box::add_item(QString item)
{
    this->addItem(item);
}

void combo_box::add_items(QStringList items)
{
    this->addItems(items);
}

void combo_box::set_current_text(QString text)
{
    this->setCurrentText(text);
}

void combo_box::mousePressEvent(QMouseEvent *event)
{
    clicked();
    QComboBox::mousePressEvent(event);
}
