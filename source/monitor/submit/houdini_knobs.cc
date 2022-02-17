// Author: Francisco Jose Contreras Cuevas
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

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
    engine_text = new text_knob("Engine Node");

    int h = HORIZONTAL_MARGIN;
    int v = VERTICAL_MARGIN;
    int s = SPACING;

    project_file->layout()->setContentsMargins(h, v, h, s);
    engine_text->layout()->setContentsMargins(h, s, h, v);

    layout->addWidget(project_file);
    layout->addWidget(engine_text);
}
