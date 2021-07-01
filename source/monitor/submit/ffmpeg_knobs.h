#ifndef FFMPEG_SUBMIT_H
#define FFMPEG_SUBMIT_H

#include <QJsonObject>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

#include "combo_box.h"
#include "file_knob.h"
#include "text_knob.h"

class ffmpeg_knobs : public QWidget
{
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

public:
    ffmpeg_knobs();

    void set_preset(QString preset_name);
    inline QString get_preset() const;
    inline QString get_command() const;
    inline QString get_movie_name() const;
};

inline QString ffmpeg_knobs::get_command() const
{
    return command_edit->text();
}

inline QString ffmpeg_knobs::get_preset() const
{
    return preset_box->get_current_text();
}

#endif // FFMPEG_SUBMIT_H
