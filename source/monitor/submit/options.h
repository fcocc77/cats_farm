#ifndef OPTIONS_H
#define OPTIONS_H

#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

#include "time_knobs.h"
#include "misc_knobs.h"
#include "shared_variables.h"

class options_class : public QWidget
{
private:
    QWidget *_monitor;
    shared_variables *shared;

    time_knobs *_time_knobs;
    misc_knobs *_misc_knobs;

    QLabel *selected_jobs_label;

    void setup_ui();
    void options_ok();
    void uptate_panel_from_job(QString job_name);

public:
    options_class(QWidget *monitor, shared_variables *shared);
    ~options_class();

    void open_panel();
    void update_panel(bool clear);
};

#endif // OPTIONS_H
