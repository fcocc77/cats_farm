#include "button.h"
#include "qt.h"
#include "util.h"
#include "../global/global.h"

button::button(QString text, QString _icon_name, bool checkable, bool checked)
    : QPushButton(text)
    , size_icon(20)
    , icon_name(_icon_name)
{
    set_icon(icon_name);

    this->setCheckable(checkable);
    set_checked(checked);

    this->setObjectName(checkable ? "button_checkable" : "toolbar_button");

    connect(this, &QPushButton::clicked, this,
            [=]() { set_checked(isChecked()); });
}

button::~button() {}

void button::change_icon(QString name)
{
    QString icon_path = VINARENDER_PATH + "/resources/images/" + name + ".png";
    QPixmap pixmap(icon_path);
    QIcon ButtonIcon(pixmap);

    this->setIcon(ButtonIcon);
    this->setIconSize(QSize(size_icon, size_icon));
}

void button::set_icon(QString name)
{
    icon_name = name;
    change_icon(icon_name + "_normal");
}

void button::set_checked(bool checked)
{
    if (!isCheckable())
        return;

    this->setChecked(checked);
    change_icon(icon_name + (checked ? "_normal" : "_disable"));
}

void button::mousePressEvent(QMouseEvent *event)
{
    if (isCheckable())
    {
        if (!isChecked())
            change_icon(icon_name + "_checked");
    }
    else
        change_icon(icon_name + "_checked");

    QPushButton::mousePressEvent(event);
}

void button::mouseReleaseEvent(QMouseEvent *event)
{
    if (isCheckable())
        change_icon(icon_name + (isChecked() ? "_normal" : "_disable"));
    else
        change_icon(icon_name + "_normal");

    QPushButton::mouseReleaseEvent(event);
}
