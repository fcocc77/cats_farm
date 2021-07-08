#include <QFileDialog>
#include <QJsonArray>
#include <QLabel>
#include <QMessageBox>
#include <QTreeWidgetItem>

#include "../global/global.h"
#include "os.h"
#include "path_utils.h"
#include "submit_widget.h"
#include "tcp.h"
#include "util.h"

submit::submit(QWidget *__monitor)
    : _monitor(__monitor)
{
    ui();
    connections();

    set_software("ffmpeg");
}

submit::~submit() {}

void submit::ui()
{
    layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    this->setObjectName("submit_widget");

    software_box = new combo_box();
    submit_button = new QPushButton("Submit");

    _ffmpeg_knobs = new ffmpeg_knobs();
    _time_knobs = new time_knobs();
    _maya_knobs = new maya_knobs();
    _houdini_knobs = new houdini_knobs();
    _misc_knobs = new misc_knobs(_monitor);

    // Settings
    QStringList items = {"Maya", "Houdini", "FFmpeg"};
    software_box->add_items(items);

    // Layout
    layout->addWidget(software_box);
    layout->addWidget(_maya_knobs);
    layout->addWidget(_houdini_knobs);
    layout->addWidget(_ffmpeg_knobs);
    layout->addWidget(_time_knobs);
    layout->addWidget(_misc_knobs);
    layout->addWidget(submit_button);
}

void submit::connections()
{
    connect(software_box, &combo_box::text_changed, this,
            &submit::set_software);

    connect(_ffmpeg_knobs, &ffmpeg_knobs::movie_changed, this,
            [=](int first_frame, int last_frame, int task_divition,
                QString job_name) {
                _time_knobs->set_first_frame(first_frame);
                _time_knobs->set_last_frame(last_frame);
                _time_knobs->set_task_divition(30);
                _misc_knobs->set_job_name(job_name);
            });

    connect(submit_button, &QPushButton::clicked, this, [this]() {
        submit_start(software_box->get_current_text().toLower());
    });
}

void submit::set_software(QString software)
{
    software = software.toLower();

    _ffmpeg_knobs->hide();
    _houdini_knobs->hide();
    _maya_knobs->hide();

    if (software == "maya")
        _maya_knobs->show();

    else if (software == "houdini")
        _houdini_knobs->show();

    else if (software == "ffmpeg")
        _ffmpeg_knobs->show();

    software_box->set_current_text(software);
}

void submit::submit_start(QString software)
{
    QJsonObject software_data;

    if (software == "ffmpeg")
        software_data = {{"input_file", _ffmpeg_knobs->get_input_file()},
                         {"output_folder", _ffmpeg_knobs->get_output_folder()},
                         {"movie_name", _ffmpeg_knobs->get_movie_name()},
                         {"command", _ffmpeg_knobs->get_command()}};

    else if (software == "maya")
        software_data = {{"scene", _maya_knobs->get_scene()},
                         {"project_folder", _maya_knobs->get_project_folder()}};

    else if (software == "houdini")
        software_data = {{"project", _houdini_knobs->get_project()},
                         {"engine", _houdini_knobs->get_engine()}};

    QJsonObject info = {
        {"job_name", _misc_knobs->get_job_name()},
        {"comment", _misc_knobs->get_comment()},
        {"paused", _misc_knobs->get_paused()},
        {"server_group", _misc_knobs->get_server_group()},
        {"priority", _misc_knobs->get_priority()},
        {"software", software},
        {"software_data", software_data},
        {"system", _linux ? "Linux" : "Windows"},
        {"first_frame", _time_knobs->get_first_frame()},
        {"last_frame", _time_knobs->get_last_frame()},
        {"task_size", _time_knobs->get_task_size()},
        {"instances", _misc_knobs->get_instances()},

    };

    bool ok = true;
    QString details = "Incomplete Fiels:\n";

    // if (job_name->text().isEmpty())
    // {
    // details += "Job Name\n";
    // ok = false;
    // }
    // if (first_frame_edit->text().isEmpty())
    // {
    // details += "First Frame\n";
    // ok = false;
    // }
    // if (last_frame_edit->text().isEmpty())
    // {
    // details += "Last Frame\n";
    // ok = false;
    // }
    // if (task_size_edit->text().isEmpty())
    // {
    // details += "Task Size\n";
    // ok = false;
    // }

    QMessageBox *msg = new QMessageBox(this);
    msg->setWindowTitle("Submit Information");

    if (ok)
    {
        send_job(info);
        msg->setText("The " + _misc_knobs->get_job_name() + " job has sended.");
        msg->show();
    }
    else
    {
        msg->setIcon(QMessageBox::Information);
        msg->setText("You must fill in all the boxes.");
        msg->setDetailedText(details);
        msg->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msg->show();
    }
}

void submit::send_job(QJsonObject info)
{
    QJsonObject settings = jread(VINARENDER_CONF_PATH + "/settings.json");
    QString host = settings["current_manager"].toString();

    tcpClient(host, MANAGER_PORT, jats({4, info}));
}

void submit::submit_file(QString file)
{
    QString ext = path_util::get_ext(file).toLower();

    QString software;
    QString job_name = path_util::basename_no_ext(file);
    QString system = _linux ? "Linux" : "Windows";

    int first_frame = 1;
    int last_frame = 100;
    int task_size = 10;

    QJsonObject software_data;

    if (ext == "mov" || ext == "mp4")
    {
        software = "FFmpeg";
        _ffmpeg_knobs->calc_ffmpeg_data(file, &first_frame, &last_frame,
                                        &task_size);

        software_data = {{"input_file", file}};
    }

    else if (ext == "mb" || ext == "ma")
    {
        software = "Maya";
        software_data = {{"scene", file}};
    }

    else if (ext == "nk")
        software = "Nuke";

    else if (ext == "vina")
        software = "VinaComp";
    else
        return;

    QJsonObject info = {
        {"job_name", job_name},
        {"comment", software},
        {"paused", true},
        {"server_group", "auto"},
        {"priority", 2},
        {"software", software},
        {"software_data", software_data},
        {"system", _linux ? "Linux" : "Windows"},
        {"first_frame", first_frame},
        {"last_frame", last_frame},
        {"task_size", task_size},
        {"instances", 1},
    };

    send_job(info);
}

void submit::submit_files(QStringList files)
{
    for (QString file : files)
        submit_file(file);
}

void submit::panel_save()
{
    QJsonObject panel = {{"software", software_box->get_current_text()},

                         {"ffmpeg_presets", _ffmpeg_knobs->get_preset()},
                         {"ffmpeg_input_file", _ffmpeg_knobs->get_input_file()},
                         {"ffmpeg_output_folder", _ffmpeg_knobs->get_output_folder()},
                         {"ffmpeg_movie_name", _ffmpeg_knobs->get_movie_name()},

                         {"maya_scene", _maya_knobs->get_scene()},
                         {"maya_project", _maya_knobs->get_project_folder()},

                         {"houdini_project", _houdini_knobs->get_project()},
                         {"houdini_engine", _houdini_knobs->get_engine()},

                         {"job_name", _misc_knobs->get_job_name()},
                         {"first_frame", _time_knobs->get_first_frame()},
                         {"last_frame", _time_knobs->get_last_frame()},
                         {"task_size", _time_knobs->get_task_size()},
                         {"priority", _misc_knobs->get_priority()},
                         {"server_group", _misc_knobs->get_server_group()},
                         {"comment", _misc_knobs->get_comment()},
                         {"paused", _misc_knobs->get_paused()},
                         {"instances", _misc_knobs->get_instances()}};

    jwrite(VINARENDER_CONF_PATH + "/submit_panel.json", panel);
}

void submit::panel_open()
{
    QJsonObject panel = jread(VINARENDER_CONF_PATH + "/submit_panel.json");

    software_box->set_current_text(panel["software"].toString(), true);

    _ffmpeg_knobs->set_preset(panel["ffmpeg_presets"].toString());
    _ffmpeg_knobs->set_input_file(panel["ffmpeg_input_file"].toString());
    _ffmpeg_knobs->set_output_folder(panel["ffmpeg_output_folder"].toString());
    _ffmpeg_knobs->set_movie_name(panel["ffmpeg_movie_name"].toString());

    _maya_knobs->set_scene(panel["maya_scene"].toString());
    _maya_knobs->set_project_folder(panel["maya_project"].toString());

    _houdini_knobs->set_project(panel["houdini_project"].toString());
    _houdini_knobs->set_engine(panel["houdini_engine"].toString());

    _time_knobs->set_first_frame(panel["first_frame"].toInt());
    _time_knobs->set_last_frame(panel["last_frame"].toInt());
    _time_knobs->set_task_size(panel["task_size"].toInt());

    _misc_knobs->set_job_name(panel["job_name"].toString());
    _misc_knobs->set_priority(panel["priority"].toInt());
    _misc_knobs->set_server_group(panel["server_group"].toString());
    _misc_knobs->set_comment(panel["comment"].toString());
    _misc_knobs->set_paused(panel["paused"].toBool());
    _misc_knobs->set_instances(panel["instances"].toInt());
}

void submit::hideEvent(QHideEvent *event)
{
    panel_save();
    QWidget::hideEvent(event);
}

void submit::showEvent(QShowEvent *event)
{
    panel_open();
    QWidget::showEvent(event);
}
