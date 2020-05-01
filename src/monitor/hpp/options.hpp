#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include "includes.hpp"

class options_class : public QWidget
{
private:
    void setup_ui();

public:
    options_class();
    ~options_class();

    QPushButton *cancel_button;
    QPushButton *ok_button;

    QLineEdit *job_name_edit;
    QComboBox *priority_combobox;
    QLineEdit *first_frame_edit;
    QLineEdit *last_frame_edit;
    QLineEdit *task_size_edit;
    QLineEdit *comment_edit;
    QLineEdit *instances_edit;
    QComboBox *group_combobox;
};

#endif // OPTIONS_HPP