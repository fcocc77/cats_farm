// Author: Francisco Jose Contreras Cuevas
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#ifndef BUTTON_H
#define BUTTON_H

#include <QPushButton>

class button : public QPushButton
{
private:
    int size_icon;
    QString icon_name;

    void change_icon(QString name);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public:
    button(QString text, QString icon_name = "", bool checkable = false,
           bool checked = true);
    ~button();

    void set_icon(QString name);
    void set_checked(bool checked);

};

#endif // BUTTON_H
