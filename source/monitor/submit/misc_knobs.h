#ifndef MISC_KNOBS_H
#define MISC_KNOBS_H

#include <QCheckBox>
#include <QWidget>

#include "combo_box.h"
#include "text_knob.h"

class misc_knobs : public QWidget
{
private:
    QWidget *_monitor;
    text_knob *job_name_edit;
    text_knob *comment_edit;
    QCheckBox *suspend_check;

    combo_box *server_group_box;
    combo_box *priority_box;

    void update_server_groups();

public:
    misc_knobs(QWidget *monitor);

    inline QString get_job_name() const;
    inline void set_job_name(QString job_name);

    inline QString get_comment() const;
    inline void set_comment(QString comment);

    inline QString get_priority() const;
    inline int get_priority_index() const;
    inline void set_priority(QString priority);

    inline bool get_suspend() const;
    inline void set_suspend(bool suspended);

    inline QString get_server_group() const;
};

inline QString misc_knobs::get_server_group() const
{
    return server_group_box->get_current_text();
}

inline QString misc_knobs::get_job_name() const
{
    return job_name_edit->get_text();
}

inline void misc_knobs::set_job_name(QString job_name)
{
    job_name_edit->set_text(job_name);
}

inline QString misc_knobs::get_comment() const
{
    return comment_edit->get_text();
}

inline void misc_knobs::set_comment(QString comment)
{
    comment_edit->set_text(comment);
}

inline QString misc_knobs::get_priority() const
{
    return priority_box->get_current_text();
}

inline int misc_knobs::get_priority_index() const
{
    return priority_box->get_index();
}

inline void misc_knobs::set_priority(QString priority)
{
    priority_box->set_current_text(priority);
}

inline bool misc_knobs::get_suspend() const
{
    return suspend_check->isChecked();
}

inline void misc_knobs::set_suspend(bool suspended)
{
    suspend_check->setChecked(suspended);
}

#endif // MISC_KNOBS_H
