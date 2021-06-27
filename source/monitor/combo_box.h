#ifndef COMBO_BOX_H
#define COMBO_BOX_H

#include <QAction>
#include <QLabel>
#include <QMenu>
#include <QWidget>

class combo_box : public QWidget
{
    Q_OBJECT
private:
    QMenu *menu;
    QLabel *label;
    QList<QAction *> actions;
    int index;

    int get_index(QString text) const;

    void mousePressEvent(QMouseEvent *event) override;

public:
    combo_box();
    ~combo_box();

    void add_item(QString item);
    void add_items(QStringList items);
    void set_current_text(QString text);
    void set_current_index(int index);
    void clear();
    QString get_current_text() const;

signals:
    void clicked();
    void current_text_changed(QString text);
};

#endif // COMBO_BOX_H
