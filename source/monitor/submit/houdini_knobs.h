// Author: Francisco Jose Contreras Cuevas
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

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

    inline QString get_project() const;
    inline void set_project(QString project);

    inline QString get_engine() const;
    inline void set_engine(QString engine);
};

inline QString houdini_knobs::get_project() const
{
    return project_file->get_path();
}

inline void houdini_knobs::set_project(QString project)
{
    project_file->set_path(project);
}

inline QString houdini_knobs::get_engine() const
{
    return engine_text->get_text();
}

inline void houdini_knobs::set_engine(QString engine)
{
    engine_text->set_text(engine);
}

#endif // HOUDINI_KNOBS_H
