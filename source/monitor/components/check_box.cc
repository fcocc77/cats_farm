// Author: Francisco Jose Contreras Cuevas
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#include <QHBoxLayout>

#include "../global/global.h"
#include "check_box.h"

check_box::check_box(QString label, bool _checked)
    : QPushButton(label)
    , checked(_checked)
{
    this->setObjectName("check_box");
    set_checked(checked);
}

void check_box::set_checked(bool _checked)
{
    checked = _checked;
    checked ? set_icon("checked", "normal") : set_icon("unchecked", "normal");
}

void check_box::set_icon(QString icon_name, QString state)
{
    QString icon_path = VINARENDER_PATH + "/resources/images/" + icon_name +
                        "_" + state + ".png";

    QPixmap pixmap(icon_path);
    QIcon ButtonIcon(pixmap);

    this->setIcon(ButtonIcon);
    this->setIconSize(QSize(20, 20));
}

void check_box::set_disabled(bool disable)
{
    this->setDisabled(disable);
    QString state = disable ? "disable" : "normal";
    checked ? set_icon("checked", state) : set_icon("unchecked", state);
}

void check_box::mousePressEvent(QMouseEvent *event)
{
    set_checked(!checked);
    QPushButton::mousePressEvent(event);
}
