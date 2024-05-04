// Author: Francisco Contreras
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include "../global/global.h"
#include "ffmpeg_knobs.h"
#include "qt.h"
#include "util.h"
#include "submit_global.h"
#include "path_utils.h"
#include "video.h"

ffmpeg_knobs::ffmpeg_knobs()
{
    setup_ui();
    open_preset();
}

void ffmpeg_knobs::setup_ui()
{
    this->setObjectName("knobs_box");
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    QWidget *command_widget = new QWidget;
    QHBoxLayout *command_layout = new QHBoxLayout(command_widget);
    QLabel *command_label = new QLabel("Args:");
    command_edit = new QLineEdit();

    QWidget *preset_widget = new QWidget;
    QHBoxLayout *preset_layout = new QHBoxLayout(preset_widget);
    QLabel *preset_label = new QLabel("Preset:");
    preset_box = new combo_box();
    preset_add = new QPushButton();
    preset_delete = new QPushButton();
    preset_name_edit = new QLineEdit();
    preset_add_button = new QPushButton("Add");
    preset_delete_button = new QPushButton("Delete");
    preset_cancel_button = new QPushButton("Cancel");

    input_file = new file_knob("Input File", "Open");
    output_folder = new file_knob("Output Folder", "Set");
    movie_name_text = new text_knob("Movie Name");

    // Layout Settings
    int h = HORIZONTAL_MARGIN;
    int v = VERTICAL_MARGIN;
    int s = SPACING;

    input_file->layout()->setContentsMargins(h, v, h, s);
    output_folder->layout()->setContentsMargins(h, s, h, s);
    movie_name_text->layout()->setContentsMargins(h, s, h, s);
    preset_layout->setContentsMargins(h, v, h, s);
    command_layout->setContentsMargins(h, s, h, v);

    preset_layout->setAlignment(Qt::AlignLeft);

    // Parametros
    command_label->setFixedWidth(INIT_LABEL_WIDTH);
    preset_label->setFixedWidth(INIT_LABEL_WIDTH);

    command_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    preset_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    preset_add->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    preset_delete->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    auto add_icon = [=](QPushButton *button, QString name) {
        qt::set_icon(
            button,
            VINARENDER_PATH + "/resources/images/" + name + "_normal.png", 15);
    };
    add_icon(preset_add_button, "add");
    add_icon(preset_delete_button, "delete");

    add_icon(preset_add, "add");
    add_icon(preset_delete, "delete");

    preset_dialog(false);

    // Connecciones
    connect(preset_add, &QPushButton::clicked, this, [=]() {
        preset_dialog(true);
        preset_delete_button->hide();
    });

    connect(preset_delete, &QPushButton::clicked, this,
            [=]() { delete_preset(); });

    connect(preset_add_button, &QPushButton::clicked, this,
            [=]() { add_preset(); });

    connect(preset_delete_button, &QPushButton::clicked, this, [=]() {
        delete_current_preset();
        preset_dialog(false);
    });

    connect(preset_cancel_button, &QPushButton::clicked, this,
            [=]() { preset_dialog(false); });

    connect(preset_box, &combo_box::text_changed,
            [=](QString text) { set_preset(text); });

    connect(command_edit, &QLineEdit::textChanged, [=](QString text) {
        QString current_command =
            presets.value(preset_box->get_current_text()).toString();

        if (current_command != text)
        {
            custom_preset = text;
            preset_box->set_index(0);
        }
    });

    connect(input_file, &file_knob::changed, this,
            &ffmpeg_knobs::update_from_file);

    // Layout
    command_layout->addWidget(command_label);
    command_layout->addWidget(command_edit);

    preset_layout->addWidget(preset_label);
    preset_layout->addWidget(preset_box);
    preset_layout->addWidget(preset_add);
    preset_layout->addWidget(preset_delete);
    preset_layout->addWidget(preset_name_edit);
    preset_layout->addWidget(preset_add_button);
    preset_layout->addWidget(preset_delete_button);
    preset_layout->addWidget(preset_cancel_button);

    layout->addWidget(input_file);
    layout->addWidget(output_folder);
    layout->addWidget(movie_name_text);
    layout->addWidget(preset_widget);
    layout->addWidget(command_widget);
}

void ffmpeg_knobs::preset_dialog(bool visible)
{
    preset_add->setVisible(!visible);
    preset_delete->setVisible(!visible);

    preset_name_edit->setVisible(visible);
    preset_add_button->setVisible(visible);
    preset_delete_button->setVisible(visible);
    preset_cancel_button->setVisible(visible);
}

void ffmpeg_knobs::add_preset()
{
    QString name = preset_name_edit->text();
    if (name.isEmpty())
        return;

    QString command = command_edit->text();
    if (command.isEmpty())
        return;

    presets.insert(name, command);

    update_preset_box();
    save_preset();
    preset_box->set_current_text(name);

    preset_dialog(false);
}

void ffmpeg_knobs::update_from_file()
{
    output_folder->set_path(os::dirname(input_file->get_path()));

    QString movie_name = path_util::basename_no_ext(input_file->get_path());
    movie_name_text->set_text(movie_name + "_output");


    int first_frame, last_frame, task_size;
    calc_ffmpeg_data(input_file->get_path(), &first_frame, &last_frame,
                     &task_size);

    movie_changed(first_frame, last_frame, 30, movie_name);
}

void ffmpeg_knobs::delete_preset()
{
    preset_dialog(true);
    preset_name_edit->hide();
    preset_add_button->hide();
}

void ffmpeg_knobs::delete_current_preset()
{
    QString preset_name = preset_box->get_current_text();
    presets.remove(preset_name);

    update_preset_box();
    save_preset();
}

void ffmpeg_knobs::update_preset_box()
{
    preset_box->clear();

    preset_box->add_item("Custom");
    preset_box->add_separator();

    QStringList preset_list;
    for (QString preset_name : presets.keys())
        preset_list.push_back(preset_name);

    preset_box->add_items(preset_list);
    preset_box->set_index(0);
}

void ffmpeg_knobs::set_preset(QString preset_name)
{
    QString command = presets.value(preset_name).toString();

    if (preset_name == "Custom")
        command = custom_preset;

    command_edit->setText(command);
    command_edit->setCursorPosition(0);
    preset_box->set_current_text(preset_name);
    preset_name_edit->setText(preset_name);
}

void ffmpeg_knobs::open_preset()
{
    presets = jread(VINARENDER_CONF_PATH + "/ffmpeg_presets.json");
    update_preset_box();
}

void ffmpeg_knobs::save_preset()
{
    jwrite(VINARENDER_CONF_PATH + "/ffmpeg_presets.json", presets);
}

void ffmpeg_knobs::calc_ffmpeg_data(QString file, int *first_frame, int *last_frame,
                              int *task_size)
{
    int frame_count = video::get_meta_data(file).frames;

    *first_frame = 0;
    *last_frame = frame_count;

    int _task_size = frame_count / 25;
    _task_size = _task_size < 50 ? 50 : _task_size;

    *task_size = _task_size;
}

void ffmpeg_knobs::set_command(QString command)
{
    command_edit->setText(command);

    for (QString preset_name : presets.keys())
    {
        QString _command = presets[preset_name].toString();
        if (_command == command)
        {
            preset_box->set_current_text(preset_name);
            break;
        }
    }
}
