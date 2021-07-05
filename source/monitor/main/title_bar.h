#ifndef TITLE_BAR_H
#define TITLE_BAR_H

#include <QWidget>
#include <QLabel>

class title_bar : public QWidget
{
private:
    QLabel *title_label;

public:
    title_bar(QString label);
};

#endif // TITLE_BAR_H
