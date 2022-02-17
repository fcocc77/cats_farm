// Author: Francisco Jose Contreras Cuevas
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#include <QVBoxLayout>
#include <QIntValidator>

#include "groups_widget.h"
#include "main_window.h"
#include "misc_knobs.h"
#include "submit_global.h"

misc_knobs::misc_knobs(QWidget *__monitor)
    : _monitor(__monitor)
{
    this->setObjectName("knobs_box");
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    job_name_edit = new text_knob("job name");
    instances_edit = new text_knob("Instances");
    comment_edit = new text_knob("Comment", false);

    paused_check = new check_box("Paused");
    paused_check->setFixedWidth(100);

    QWidget *priority_and_group = new QWidget();
    QHBoxLayout *priority_and_group_layout = new QHBoxLayout(priority_and_group);
    QLabel *server_group_label = new QLabel("Server Group:");
    QLabel *priority_label = new QLabel("Priority:");

    server_group_box = new combo_box();
    priority_box = new combo_box();

    QWidget *paused_widget = new QWidget();
    QHBoxLayout *paused_layout = new QHBoxLayout(paused_widget);
    QLabel *paused_label = new QLabel();

    // Ajustes
    comment_edit->set_placeholder_text("Write your comment here");
    job_name_edit->set_placeholder_text("Name");

    instances_edit->get_line_edit()->setValidator(
        new QIntValidator(1, 16, this));
    instances_edit->get_line_edit()->setPlaceholderText("Number 1-16");

    priority_box->add_items({"Very High", "High", "Normal", "Low", "Very Low"});
    priority_box->set_index(2);

    int h = HORIZONTAL_MARGIN;
    int v = VERTICAL_MARGIN;
    int s = SPACING;

    priority_and_group_layout->setContentsMargins(h, v, h, s);
    instances_edit->layout()->setContentsMargins(h, s, h, s);
    paused_layout->setContentsMargins(h, s, h, v);
    job_name_edit->layout()->setContentsMargins(h, v, h, s);
    comment_edit->layout()->setContentsMargins(h, s, h, v);

    instances_edit->setFixedWidth(246);

    priority_and_group_layout->setAlignment(Qt::AlignLeft);
    paused_layout->setAlignment(Qt::AlignLeft);

    paused_label->setFixedWidth(INIT_LABEL_WIDTH);

    server_group_label->setFixedWidth(INIT_LABEL_WIDTH);
    server_group_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    priority_label->setFixedWidth(70);
    priority_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    server_group_box->setMinimumWidth(100);

    // Conecciones
    connect(server_group_box, &combo_box::clicked, this,
            &misc_knobs::update_server_groups);

    // Layout
    priority_and_group_layout->addWidget(server_group_label);
    priority_and_group_layout->addWidget(server_group_box);
    priority_and_group_layout->addWidget(priority_label);
    priority_and_group_layout->addWidget(priority_box);

    paused_layout->addWidget(paused_label);
    paused_layout->addWidget(paused_check);

    layout->addWidget(priority_and_group);
    layout->addWidget(instances_edit);
    layout->addWidget(paused_widget);
    layout->addWidget(job_name_edit);
    layout->addWidget(comment_edit);
}

void misc_knobs::update_server_groups()
{
    groups_class *groups = static_cast<groups_class *>(
        static_cast<monitor *>(_monitor)->get_groups_widget());

    server_group_box->clear();
    server_group_box->add_items({"None"});

    for (QString group : groups->get_groups())
        server_group_box->add_items({group});
}

void misc_knobs::clear()
{
    job_name_edit->clear();
    comment_edit->clear();
    instances_edit->clear();
}

void misc_knobs::set_disabled(bool disable, QString knob_name)
{
    this->setDisabled(disable);

    job_name_edit->set_disabled(disable);
    comment_edit->set_disabled(disable);
    instances_edit->set_disabled(disable);

    server_group_box->set_disabled(disable);
    priority_box->set_disabled(disable);
    paused_check->set_disabled(disable);
}
