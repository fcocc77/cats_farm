#include "button.h"
#include "util.h"
#include "../global/global.h"

button::button(QString text, QString _icon_name)
    : QPushButton(text)
    , size_icon(22)
    , icon_name(_icon_name)
{
    set_icon(icon_name);
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
