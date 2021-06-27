#ifndef COMBO_BOX_H
#define COMBO_BOX_H

#include <QComboBox>

class combo_box : public QComboBox
{
    Q_OBJECT
public:
    combo_box();
    ~combo_box();

    void add_item(QString item);
    void add_items(QStringList items);
    void set_current_text(QString text);

    void mousePressEvent(QMouseEvent *event) override;

signals:
    void clicked();
};

#endif // COMBO_BOX_H
