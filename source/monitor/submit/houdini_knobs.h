#ifndef HOUDINI_KNOBS_H
#define HOUDINI_KNOBS_H

#include <QWidget>

#include "file_knob.h"
#include "text_knob.h"

class houdini_knobs : public QWidget
{
private:
    file_knob *project_file;
    text_knob *engine_text;

public:
    houdini_knobs();
};

#endif // HOUDINI_KNOBS_H
