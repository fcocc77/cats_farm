#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include "../global/global.h"
#include "ffmpeg_submit.h"
#include "qt.h"
#include "util.h"

ffmpeg_submit::ffmpeg_submit(QList<int> params)
    : label_width(params[0])
    , h_margin(params[1])
    , v_margin(params[2])
    , v_padding(params[3])
{
    setup_ui();
    open_preset();
}

void ffmpeg_submit::setup_ui()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    QWidget *command_widget = new QWidget;
    QHBoxLayout *command_layout = new QHBoxLayout(command_widget);
    QLabel *command_label = new QLabel("Command");
    command_edit = new QLineEdit();

    QWidget *preset_widget = new QWidget;
    QHBoxLayout *preset_layout = new QHBoxLayout(preset_widget);
    QLabel *preset_label = new QLabel("Preset");
    preset_box = new combo_box();
    preset_add = new QPushButton();
    preset_delete = new QPushButton();
    preset_name = new QLineEdit();
    preset_add_button = new QPushButton("Add");
    preset_delete_button = new QPushButton("Delete");
    preset_cancel_button = new QPushButton("Cancel");

    // Layout Settings
    preset_layout->setContentsMargins(h_margin, v_padding, h_margin, v_margin);
    command_layout->setContentsMargins(h_margin, v_margin, h_margin, v_padding);

    preset_layout->setAlignment(Qt::AlignLeft);

    // Parametros
    command_label->setFixedWidth(label_width);
    preset_label->setFixedWidth(label_width);

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

    connect(preset_box, &combo_box::current_text_changed,
            [=](QString text) { set_preset(text); });

    connect(command_edit, &QLineEdit::textChanged, [=](QString text) {
        QString current_command =
            presets[preset_box->get_current_text()].toString();

        if (current_command != text)
        {
            custom_preset = text;
            preset_box->set_index(0);
        }
    });

    // Layout
    command_layout->addWidget(command_label);
    command_layout->addWidget(command_edit);

    preset_layout->addWidget(preset_label);
    preset_layout->addWidget(preset_box);
    preset_layout->addWidget(preset_add);
    preset_layout->addWidget(preset_delete);
    preset_layout->addWidget(preset_name);
    preset_layout->addWidget(preset_add_button);
    preset_layout->addWidget(preset_delete_button);
    preset_layout->addWidget(preset_cancel_button);

    layout->addWidget(preset_widget);
    layout->addWidget(command_widget);
}

void ffmpeg_submit::preset_dialog(bool visible)
{
    preset_add->setVisible(!visible);
    preset_delete->setVisible(!visible);

    preset_name->setVisible(visible);
    preset_add_button->setVisible(visible);
    preset_delete_button->setVisible(visible);
    preset_cancel_button->setVisible(visible);
}

void ffmpeg_submit::add_preset()
{
    QString name = preset_name->text();
    if (name.isEmpty())
        return;

    QString command = command_edit->text();
    if (command.isEmpty())
        return;

    presets.insert(name, command);

    update_preset_box();
    save_preset();

    preset_dialog(false);
}

void ffmpeg_submit::delete_preset()
{
    preset_dialog(true);
    preset_name->hide();
    preset_add_button->hide();
}

void ffmpeg_submit::delete_current_preset()
{
    QString preset_name = preset_box->get_current_text();
    presets.remove(preset_name);

    update_preset_box();
    save_preset();
}

void ffmpeg_submit::update_preset_box()
{
    preset_box->clear();

    preset_box->add_item("None");
    preset_box->add_separator();

    QStringList preset_list;
    for (QString preset_name : presets.keys())
        preset_list.push_back(preset_name);

    preset_box->add_items(preset_list);
    preset_box->set_index(0);
}

void ffmpeg_submit::set_preset(QString preset_name)
{
    QString command = presets[preset_name].toString();

    if (preset_name == "None")
        command = custom_preset;

    command_edit->setText(command);
    command_edit->setCursorPosition(0);
}

void ffmpeg_submit::open_preset()
{
    presets = jread(VINARENDER_CONF_PATH + "/ffmpeg_presets.json");
    update_preset_box();
}

void ffmpeg_submit::save_preset()
{
    jwrite(VINARENDER_CONF_PATH + "/ffmpeg_presets.json", presets);
}
