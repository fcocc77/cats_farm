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

    suspended_check = new check_box("Suspended");
    suspended_check->setFixedWidth(100);

    QWidget *priority_and_group = new QWidget();
    QHBoxLayout *priority_and_group_layout = new QHBoxLayout(priority_and_group);
    QLabel *server_group_label = new QLabel("Server Group:");
    QLabel *priority_label = new QLabel("Priority:");

    server_group_box = new combo_box();
    priority_box = new combo_box();

    QWidget *suspended_widget = new QWidget();
    QHBoxLayout *suspended_layout = new QHBoxLayout(suspended_widget);
    QLabel *suspended_label = new QLabel();

    // Ajustes
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
    suspended_layout->setContentsMargins(h, s, h, v);
    job_name_edit->layout()->setContentsMargins(h, v, h, s);
    comment_edit->layout()->setContentsMargins(h, s, h, v);

    instances_edit->setFixedWidth(246);

    priority_and_group_layout->setAlignment(Qt::AlignLeft);
    suspended_layout->setAlignment(Qt::AlignLeft);

    suspended_label->setFixedWidth(INIT_LABEL_WIDTH);

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

    suspended_layout->addWidget(suspended_label);
    suspended_layout->addWidget(suspended_check);

    layout->addWidget(priority_and_group);
    layout->addWidget(instances_edit);
    layout->addWidget(suspended_widget);
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
