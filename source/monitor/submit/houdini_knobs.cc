#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

#include "houdini_knobs.h"
#include "submit_global.h"

houdini_knobs::houdini_knobs()
{
    this->setObjectName("knobs_box");
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    project_file = new file_knob("Scene File", "Open");

    QWidget *engine_widget = new QWidget();
    QHBoxLayout *engine_layout = new QHBoxLayout(engine_widget);
    QLabel *engine_label = new QLabel("Engine Node:");
    QLineEdit *engine_edit = new QLineEdit();

    int h = HORIZONTAL_MARGIN;
    int v = VERTICAL_MARGIN;
    int s = SPACING;

    project_file->layout()->setContentsMargins(h, v, h, s);
    engine_layout->setContentsMargins(h, s, h, v);

    engine_label->setFixedWidth(INIT_LABEL_WIDTH);
    engine_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    engine_layout->addWidget(engine_label);
    engine_layout->addWidget(engine_edit);

    layout->addWidget(project_file);
    layout->addWidget(engine_widget);
}
