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

    QString basename = checked ? "checked_normal.png" : "unchecked_normal.png";
    QString icon_path = VINARENDER_PATH + "/resources/images/" + basename;

    QPixmap pixmap(icon_path);
    QIcon ButtonIcon(pixmap);

    this->setIcon(ButtonIcon);
    this->setIconSize(QSize(20, 20));
}

void check_box::mousePressEvent(QMouseEvent *event)
{
    set_checked(!checked);
    QPushButton::mousePressEvent(event);
}
