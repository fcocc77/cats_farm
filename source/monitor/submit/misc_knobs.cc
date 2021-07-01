#include <QVBoxLayout>

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

    job_name = new text_knob("Job Name");
    comment_edit = new text_knob("Comment", false);

    suspend_check = new QCheckBox("Suspended");

    QWidget *server_group_widget = new QWidget();
    QHBoxLayout *server_group_layout = new QHBoxLayout(server_group_widget);
    QLabel *server_group_label = new QLabel("Server Group:");
    server_group_box = new combo_box();

    QWidget *priority_widget = new QWidget();
    QHBoxLayout *priority_layout = new QHBoxLayout(priority_widget);
    QLabel *priority_label = new QLabel("Priority:");
    priority_box = new combo_box();

    // Ajustes
    priority_box->add_items({"Very High", "High", "Normal", "Low", "Very Low"});
    priority_box->set_index(2);

    int h = HORIZONTAL_MARGIN;
    int v = VERTICAL_MARGIN;
    int s = SPACING;

    job_name->layout()->setContentsMargins(h, v, h, s);
    comment_edit->layout()->setContentsMargins(h, s, h, v);
    server_group_layout->setContentsMargins(h, s, h, s);
    priority_layout->setContentsMargins(h, s, h, s);

    priority_layout->setAlignment(Qt::AlignLeft);
    server_group_layout->setAlignment(Qt::AlignLeft);

    server_group_label->setFixedWidth(INIT_LABEL_WIDTH);
    server_group_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    priority_label->setFixedWidth(INIT_LABEL_WIDTH);
    priority_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    // Conecciones
    connect(server_group_box, &combo_box::clicked, this,
            &misc_knobs::update_server_groups);

    // Layout
    server_group_layout->addWidget(server_group_label);
    server_group_layout->addWidget(server_group_box);

    priority_layout->addWidget(priority_label);
    priority_layout->addWidget(priority_box);

    layout->addWidget(job_name);
    layout->addWidget(server_group_widget);
    layout->addWidget(priority_widget);
    layout->addWidget(suspend_check);
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
