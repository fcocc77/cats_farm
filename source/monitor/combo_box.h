#ifndef COMBO_BOX_H
#define COMBO_BOX_H

#include <QComboBox>

class combo_box : public QComboBox
{
public:
    combo_box();
    ~combo_box();

    void add_item(QString item);
    void add_items(QStringList items);
    void set_current_text(QString text);
};

#endif // COMBO_BOX_H
