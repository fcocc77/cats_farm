#ifndef MAYA_KNOBS_H
#define MAYA_KNOBS_H

#include <QWidget>

#include "file_knob.h"

class maya_knobs : public QWidget
{
private:
    file_knob *scene_file;
    file_knob *project_file;

public:
    maya_knobs();
};

#endif // MAYA_KNOBS_H
