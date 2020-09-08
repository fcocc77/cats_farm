#include <options.hpp>

options_class::options_class()
{
    setup_ui();
}

options_class::~options_class()
{
}

void options_class::setup_ui()
{
    this->setObjectName("options");

    QVBoxLayout *main_layout = new QVBoxLayout();
    this->setLayout(main_layout);
    {
        QLayout *first_knobs = qlayout(main_layout, "v", "first_knobs");
        {
            QLayout *name_knobs = qlayout(first_knobs, "h");
            {
                QLabel *name_label = new QLabel("Job Name:");
                name_knobs->addWidget(name_label);

                job_name_edit = new QLineEdit();
                name_knobs->addWidget(job_name_edit);
            }
            QLayout *priority_knobs = qlayout(first_knobs, "h");
            {
                QLabel *priority_label = new QLabel("Priority:");
                priority_knobs->addWidget(priority_label);

                priority_combobox = new QComboBox();
                priority_combobox->addItem("Very High");
                priority_combobox->addItem("High");
                priority_combobox->addItem("Normal");
                priority_combobox->addItem("Low");
                priority_combobox->addItem("Very Low");
                priority_knobs->addWidget(priority_combobox);

                QLabel *group_label = new QLabel("Server Group:");
                priority_knobs->addWidget(group_label);

                group_combobox = new QComboBox();
                priority_knobs->addWidget(group_combobox);
            }
        }

        QLayout *second_knobs = qlayout(main_layout, "v", "second_knobs");
        {
            QLayout *range_knobs = qlayout(second_knobs, "h");
            {
                QLabel *range_label = new QLabel("Frame Range:");
                range_knobs->addWidget(range_label);

                first_frame_edit = new QLineEdit();
                range_knobs->addWidget(first_frame_edit);

                last_frame_edit = new QLineEdit();
                range_knobs->addWidget(last_frame_edit);
            }
            QLayout *task_knobs = qlayout(second_knobs, "h");
            {
                QLabel *task_size_label = new QLabel("Task Size:");
                task_knobs->addWidget(task_size_label);

                task_size_edit = new QLineEdit();
                task_knobs->addWidget(task_size_edit);
            }
            QLayout *instances_knobs = qlayout(second_knobs, "h");
            {
                QLabel *instances_label = new QLabel("Instances:");
                instances_knobs->addWidget(instances_label);

                instances_edit = new QLineEdit();
                instances_knobs->addWidget(instances_edit);
            }
        }

        QLayout *third_knobs = qlayout(main_layout, "v", "third_knobs");
        {
            QLayout *comment_knobs = qlayout(third_knobs, "h");
            {
                QLabel *comment_label = new QLabel("Comment:");
                comment_knobs->addWidget(comment_label);

                comment_edit = new QLineEdit();
                comment_knobs->addWidget(comment_edit);
            }
        }

        QLayout *fourth_knobs = qlayout(main_layout, "v");
        {
            QLayout *button_knobs = qlayout(fourth_knobs, "h");
            {
                cancel_button = new QPushButton("Cancel");
                button_knobs->addWidget(cancel_button);

                ok_button = new QPushButton("OK");
                button_knobs->addWidget(ok_button);
            }
        }
    }
}