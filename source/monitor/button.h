#ifndef BUTTON_H
#define BUTTON_H

#include <QPushButton>

class button : public QPushButton
{
private:
    int size_icon;
    QString icon_name;

    void change_icon(QString name);

public:
    button(QString text, QString icon_name = "");
    ~button();

    void set_icon(QString name);

};

#endif // BUTTON_H
