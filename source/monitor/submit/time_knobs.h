#ifndef TIME_KNOBS_H
#define TIME_KNOBS_H

#include <QLineEdit>
#include <QWidget>
#include <QLabel>

#include "combo_box.h"

class time_knobs : public QWidget
{
private:
    QLineEdit *first_frame_edit, *last_frame_edit;
    QLineEdit *task_size_edit;
    combo_box *task_size_box;

    QLabel *frame_range_label, *task_size_label;

    void calculate_task_size(QString item_text);
    void calculate_task_divition(int task_size);

public:
    time_knobs();

    void set_disabled(bool disable, QString knob_name = "");

    inline int get_first_frame() const;
    inline int get_last_frame() const;
    inline int get_task_size() const;

    inline void set_first_frame(int first_frame);
    inline void set_last_frame(int last_frame);
    inline void set_task_size(int task_size);

    inline void set_task_divition(int divitions);

    void clear();
};

inline void time_knobs::set_task_divition(int divitions)
{
    task_size_box->set_current_text(QString::number(divitions) + " Tasks",
                                    true);
}

inline void time_knobs::set_first_frame(int first_frame)
{
    first_frame_edit->setText(QString::number(first_frame));
}

inline void time_knobs::set_last_frame(int last_frame)
{
    last_frame_edit->setText(QString::number(last_frame));
}

inline void time_knobs::set_task_size(int task_size)
{
    task_size_edit->setText(QString::number(task_size));
}

inline int time_knobs::get_first_frame() const
{
    return first_frame_edit->text().toInt();
}

inline int time_knobs::get_last_frame() const
{
    return last_frame_edit->text().toInt();
}

inline int time_knobs::get_task_size() const
{
    return task_size_edit->text().toInt();
}

#endif // TIME_KNOBS_H
