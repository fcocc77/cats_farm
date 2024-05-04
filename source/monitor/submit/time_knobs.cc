// Author: Francisco Contreras
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#include <QHBoxLayout>
#include <QIntValidator>
#include <QLabel>

#include "time_knobs.h"

time_knobs::time_knobs()
{
    this->setObjectName("knobs_box");
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    QWidget *frame_range_widget = new QWidget();
    QWidget *task_size_widget = new QWidget();

    QHBoxLayout *frame_range_layout = new QHBoxLayout(frame_range_widget);
    QHBoxLayout *task_size_layout = new QHBoxLayout(task_size_widget);

    frame_range_label = new QLabel("Frame Range:");
    task_size_label = new QLabel("Task Size:");

    first_frame_edit = new QLineEdit();
    last_frame_edit = new QLineEdit();

    task_size_box = new combo_box();
    task_size_edit = new QLineEdit();

    // Ajustes
    first_frame_edit->setPlaceholderText("First Frame");
    last_frame_edit->setPlaceholderText("Last Frame");
    task_size_edit->setPlaceholderText("Task Size");

    first_frame_edit->setValidator(new QIntValidator(-100000, 100000, this));
    last_frame_edit->setValidator(new QIntValidator(-100000, 100000, this));
    task_size_edit->setValidator(new QIntValidator(0, 100000, this));

    task_size_box->add_items({"Custom", "5 Tasks", "10 Tasks", "15 Tasks",
                              "20 Tasks", "30 Tasks", "40 Tasks", "50 Tasks"});
    task_size_box->set_index(0);
    task_size_box->setFixedWidth(120);
    task_size_box->setToolTip("Task divisions: calculate the size of the task "
                              "from the number of divisions");

    int label_width = 100;

    frame_range_label->setFixedWidth(label_width);
    task_size_label->setFixedWidth(label_width);

    frame_range_layout->setContentsMargins(20, 15, 20, 3);
    task_size_layout->setContentsMargins(20, 3, 20, 15);

    frame_range_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    task_size_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    // Conecciones
    connect(task_size_box, &combo_box::text_changed, this,
            &time_knobs::calculate_task_size);

    connect(task_size_edit, &QLineEdit::textChanged, this,
            [=](QString text) { calculate_task_divition(text.toInt()); });

    connect(first_frame_edit, &QLineEdit::textChanged, this, [=](QString text) {
        calculate_task_size(task_size_box->get_current_text());
    });

    connect(last_frame_edit, &QLineEdit::textChanged, this, [=](QString text) {
        calculate_task_size(task_size_box->get_current_text());
    });

    // Layout
    frame_range_layout->addWidget(frame_range_label);
    frame_range_layout->addWidget(first_frame_edit);
    frame_range_layout->addWidget(last_frame_edit);

    task_size_layout->addWidget(task_size_label);
    task_size_layout->addWidget(task_size_box);
    task_size_layout->addWidget(task_size_edit);

    layout->addWidget(frame_range_widget);
    layout->addWidget(task_size_widget);
}

void time_knobs::calculate_task_size(QString item_text)
{
    int divisions = item_text.split("Tasks")[0].toInt();

    if (!divisions)
        return;

    int first_frame = first_frame_edit->text().toInt();
    int last_frame = last_frame_edit->text().toInt();
    int frame_count = last_frame - first_frame + 1;

    int task_size = frame_count / divisions;

    task_size_edit->setText(QString::number(task_size));
}

void time_knobs::calculate_task_divition(int task_size)
{
    if (!task_size)
        return;

    int first_frame = first_frame_edit->text().toInt();
    int last_frame = last_frame_edit->text().toInt();

    int frame_count = last_frame - first_frame + 1;

    int divisions = frame_count / task_size;
    task_size_box->set_label_text(QString::number(divisions) + " Tasks");
}

void time_knobs::set_disabled(bool disable, QString knob_name)
{
    first_frame_edit->setDisabled(disable);
    last_frame_edit->setDisabled(disable);
    task_size_edit->setDisabled(disable);

    frame_range_label->setDisabled(disable);
    task_size_label->setDisabled(disable);

    task_size_box->set_disabled(disable);
}

void time_knobs::clear()
{
    first_frame_edit->clear();
    last_frame_edit->clear();
    task_size_edit->clear();
}
