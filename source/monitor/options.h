#ifndef OPTIONS_H
#define OPTIONS_H

#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>

class options_class : public QWidget
{
private:
    QWidget *_monitor;

    void setup_ui();
    void options_ok();

public:
    options_class(QWidget *monitor);
    ~options_class();

    void open_panel();
    void update_panel();
};

#endif // OPTIONS_H
