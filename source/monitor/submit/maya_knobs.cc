// Author: Francisco Contreras
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#include <QVBoxLayout>

#include "maya_knobs.h"
#include "submit_global.h"

maya_knobs::maya_knobs()
{
    this->setObjectName("knobs_box");
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    scene_file = new file_knob("Project Folder", "Set", true);
    project_dir = new file_knob("Scene File", "Open");

    int h = HORIZONTAL_MARGIN;
    int v = VERTICAL_MARGIN;
    int s = SPACING;

    scene_file->layout()->setContentsMargins(h, v, h, s);
    project_dir->layout()->setContentsMargins(h, s, h, v);

    layout->addWidget(scene_file);
    layout->addWidget(project_dir);
}
