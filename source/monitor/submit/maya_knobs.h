#ifndef MAYA_KNOBS_H
#define MAYA_KNOBS_H

#include <QWidget>

#include "file_knob.h"

class maya_knobs : public QWidget
{
private:
    file_knob *scene_file;
    file_knob *project_dir;

public:
    maya_knobs();

    inline QString get_scene() const;
    inline void set_scene(QString scene_path);

    inline QString get_project_folder() const;
    inline void set_project_folder(QString project_dir);
};

inline QString maya_knobs::get_scene() const
{
    return scene_file->get_path();
}

inline void maya_knobs::set_scene(QString scene_path)
{
    scene_file->set_path(scene_path);
}

inline QString maya_knobs::get_project_folder() const
{
    return project_dir->get_path();
}

inline void maya_knobs::set_project_folder(QString project)
{
    project_dir->set_path(project);
}

#endif // MAYA_KNOBS_H
