#ifndef HOUDINI_KNOBS_H
#define HOUDINI_KNOBS_H

#include <QWidget>

#include "file_knob.h"

class houdini_knobs : public QWidget
{
private:
    file_knob *project_file;

public:
    houdini_knobs();
};

#endif // HOUDINI_KNOBS_H
