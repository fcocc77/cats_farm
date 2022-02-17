// Author: Francisco Jose Contreras Cuevas
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#ifndef FFMPEG_SUBMIT_H
#define FFMPEG_SUBMIT_H

#include <QJsonObject>
#include <QLineEdit>
#include <QPainter>
#include <QPushButton>
#include <QStyleOption>
#include <QWidget>

#include "combo_box.h"
#include "file_knob.h"
#include "text_knob.h"

class ffmpeg_knobs : public QWidget
{
    Q_OBJECT
#include "q_object.h"

private:
    QPushButton *preset_add_button, *preset_delete_button,
        *preset_cancel_button;
    QPushButton *preset_add, *preset_delete;
    QLineEdit *preset_name_edit, *command_edit;

    combo_box *preset_box;
    QJsonObject presets;
    QString custom_preset;

    file_knob *input_file, *output_folder;
    text_knob *movie_name_text;

    void setup_ui();
    void preset_dialog(bool visible);

    void add_preset();
    void delete_preset();

    void open_preset();
    void save_preset();
    void update_preset_box();
    void delete_current_preset();

    void update_from_file();

public:
    ffmpeg_knobs();

    void set_preset(QString preset_name);
    void set_command(QString command);

    inline QString get_preset() const;
    inline QString get_command() const;

    inline QString get_input_file() const;
    inline void set_input_file(QString filename);

    inline QString get_output_folder() const;
    inline void set_output_folder(QString output_folder);

    inline QString get_movie_name() const;
    inline void set_movie_name(QString movie_name);

    void calc_ffmpeg_data(QString file, int *first_frame, int *last_frame,
                          int *task_size);

signals:
    void movie_changed(int first_frame, int last_frame, int task_divition,
                       QString job_name);
};

inline QString ffmpeg_knobs::get_input_file() const
{
    return input_file->get_path();
}

inline void ffmpeg_knobs::set_input_file(QString filename)
{
    input_file->set_path(filename);
}

inline QString ffmpeg_knobs::get_output_folder() const
{
    return output_folder->get_path();
}

inline void ffmpeg_knobs::set_output_folder(QString _output_folder)
{
    output_folder->set_path(_output_folder);
}

inline QString ffmpeg_knobs::get_movie_name() const
{
    return movie_name_text->get_text();
}

inline void ffmpeg_knobs::set_movie_name(QString movie_name)
{
    movie_name_text->set_text(movie_name);
}

inline QString ffmpeg_knobs::get_command() const
{
    return command_edit->text();
}

inline QString ffmpeg_knobs::get_preset() const
{
    return preset_box->get_current_text();
}

#endif // FFMPEG_SUBMIT_H
