#ifndef MISC_KNOBS_H
#define MISC_KNOBS_H

#include <QWidget>
#include <QCheckBox>

#include "combo_box.h"
#include "text_knob.h"

class misc_knobs : public QWidget
{
private:
    QWidget *_monitor;
    text_knob *job_name;
    text_knob *comment_edit;
    QCheckBox *suspend_check;

    combo_box *server_group_box;
    combo_box *priority_box;

    void update_server_groups();

public:
    misc_knobs(QWidget *monitor);
};

#endif // MISC_KNOBS_H
