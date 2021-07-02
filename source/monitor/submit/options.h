#ifndef OPTIONS_H
#define OPTIONS_H

#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>

#include "time_knobs.h"
#include "misc_knobs.h"

class options_class : public QWidget
{
private:
    QWidget *_monitor;

    time_knobs *_time_knobs;
    misc_knobs *_misc_knobs;

    void setup_ui();
    void options_ok();

public:
    options_class(QWidget *monitor);
    ~options_class();

    void open_panel();
    void update_panel();
};

#endif // OPTIONS_H
