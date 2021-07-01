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
#include "video.h"

submit::submit(QWidget *__monitor)
    : _monitor(__monitor)
{
    ui();
    connections();

    set_software("ffmpeg");
    panel_open();
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

    // connect(project_button, &QPushButton::clicked, this, [this]() {
    // QString file_name = QFileDialog::getOpenFileName(
    // _monitor, "Project File", project_edit->text());

    // if (not file_name.isEmpty())
    // project_edit->setText(file_name);

    // update_ffmpeg_panel();
    // });

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
    QString system = _linux ? "Linux" : "Windows";
    QString misc;

    // if (software == "ffmpeg")
    // {
    // QJsonObject misc_obj = {{"output_dir", project_dir_edit->text()},
    // {"movie_name", render_node_edit->text()},
    // {"command", _ffmpeg_knobs->get_command()}};
    // misc = jots(misc_obj);
    // }

    // QJsonArray info = {job_name->text(),
    // "",
    // server_group_box->get_current_text(),
    // _time_knobs->get_first_frame(),
    // _time_knobs->get_last_frame(),
    // _time_knobs->get_task_size(),
    // priority->get_current_text(),
    // suspend_box->isChecked(),
    // comment_edit->text(),
    // software,
    // project_edit->text(),
    // misc,
    // system,
    // 1,
    // render_node_edit->text()};

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
        // send_job(info);
        // msg->setText("The " + job_name->text() + " job has sended.");
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

void submit::send_job(QJsonArray info)
{
    QJsonObject settings = jread(VINARENDER_CONF_PATH + "/settings.json");
    int port = settings["manager"].toObject()["port"].toInt();
    QString host = settings["current_manager"].toString();

    tcpClient(host, port, jats({4, info}));
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

    if (ext == "mov" || ext == "mp4")
    {
        software = "FFmpeg";
        calc_ffmpeg_data(file, &first_frame, &last_frame, &task_size);
    }

    else if (ext == "mb" || ext == "ma")
        software = "Maya";

    else if (ext == "nk")
        software = "Nuke";

    else if (ext == "vina")
        software = "VinaComp";
    else
        return;

    QJsonArray info = {job_name,  "",       "auto", first_frame, last_frame,
                       task_size, "Normal", true,   software,    software,
                       file,      "",       system, 1,           ""};

    send_job(info);
}

void submit::submit_files(QStringList files)
{
    for (QString file : files)
        submit_file(file);
}

void submit::panel_save()
{
    // QJsonObject panel = {{"software", software_box->get_current_text()},
    // {"ffmpeg_presets", _ffmpeg_knobs->get_preset()},
    // {"job_name", job_name->text()},
    // {"first_frame", _time_knobs->get_first_frame()},
    // {"last_frame", _time_knobs->get_last_frame()},
    // {"task_size", _time_knobs->get_task_size()},
    // {"priority", priority->get_current_text()},
    // {"server_group", server_group_box->get_current_text()},
    // {"comment", comment_edit->text()},
    // {"suspend", suspend_box->isChecked()}};

    // jwrite(VINARENDER_CONF_PATH + "/submit_panel.json", panel);
}

void submit::panel_open()
{
    QJsonObject panel = jread(VINARENDER_CONF_PATH + "/submit_panel.json");

    software_box->set_current_text(panel["software"].toString(), true);
    _ffmpeg_knobs->set_preset(panel["ffmpeg_presets"].toString());

    // project_dir_edit->setText(panel["project_dir"].toString());
    // project_edit->setText(panel["project"].toString());
    // render_node_edit->setText(panel["render_node"].toString());

    // job_name->setText(panel["job_name"].toString());

    // _time_knobs->set_first_frame(panel["first_frame"].toInt());
    // _time_knobs->set_last_frame(panel["last_frame"].toInt());
    // _time_knobs->set_task_size(panel["task_size"].toInt());

    // priority->set_current_text(panel["priority"].toString());
    // server_group_box->set_current_text(panel["server_group"].toString());

    // comment_edit->setText(panel["comment"].toString());
    // suspend_box->setChecked(panel["suspend"].toBool());
}

void submit::update_ffmpeg_panel()
{
    // if (software_box->get_current_text().toLower() != "ffmpeg")
    // return;

    // QString src_movie = project_edit->text();
    // project_dir_edit->setText(os::dirname(src_movie));

    // QString movie_name = path_util::basename_no_ext(src_movie);
    // render_node_edit->setText(movie_name + "_output");

    // job_name->setText(movie_name);

    // int first_frame, last_frame, task_size;
    // calc_ffmpeg_data(src_movie, &first_frame, &last_frame, &task_size);

    // _time_knobs->set_first_frame(first_frame);
    // _time_knobs->set_last_frame(last_frame);

    // _time_knobs->set_task_divition(30);
}

void submit::calc_ffmpeg_data(QString file, int *first_frame, int *last_frame,
                              int *task_size)
{
    int frame_count = video::get_meta_data(file).frames;

    *first_frame = 0;
    *last_frame = frame_count;

    int _task_size = frame_count / 25;
    _task_size = _task_size < 50 ? 50 : _task_size;

    *task_size = _task_size;
}

void submit::hideEvent(QHideEvent *event)
{
    panel_save();
    QWidget::hideEvent(event);
}
