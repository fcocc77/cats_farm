#include <QFileDialog>
#include <QJsonArray>
#include <QLabel>
#include <QMessageBox>
#include <QScrollArea>
#include <QTreeWidgetItem>

#include "../global/global.h"
#include "groups.h"
#include "main_window.h"
#include "os.h"
#include "submit.h"
#include "util.h"
#include "tcp.h"
#include "path_utils.h"
#include "video.h"

submit::submit(QWidget *__monitor)
    : _monitor(__monitor)
{
    layout = new QVBoxLayout(this);
    layout->setMargin(0);

    this->setObjectName("submit_widget");

    software_box = new combo_box();

    project_dir_edit = new QLineEdit();
    project_dir_button = new QPushButton("Open");

    project_button = new QPushButton("Open");
    project_edit = new QLineEdit();

    render_node_edit = new QLineEdit();

    job_name = new QLineEdit();
    server_group_box = new combo_box();
    priority = new combo_box();
    task_size_edit = new QLineEdit();
    comment_edit = new QLineEdit();
    first_frame_edit = new QLineEdit();
    last_frame_edit = new QLineEdit();
    suspend_box = new QCheckBox("Suspended");
    submit_button = new QPushButton("Submit");

    ui();
    connections();

    set_software("ffmpeg");
    panel_open();
}

submit::~submit() {}

void submit::ui()
{
    int label_width = 100;
    int h_margin = 20;
    int v_margin = 3;
    int v_padding = 15;

    QWidget *main_widget = new QWidget();
    QVBoxLayout *main_layout = new QVBoxLayout(main_widget);

    QWidget *box_a_widget = new QWidget();
    QVBoxLayout *box_a_layout = new QVBoxLayout(box_a_widget);

    QWidget *box_b_widget = new QWidget();
    QVBoxLayout *box_b_layout = new QVBoxLayout(box_b_widget);

    QWidget *box_c_widget = new QWidget();
    QVBoxLayout *box_c_layout = new QVBoxLayout(box_c_widget);

    QWidget *box_d_widget = new QWidget();
    QVBoxLayout *box_d_layout = new QVBoxLayout(box_d_widget);

    QWidget *box_e_widget = new QWidget();
    QVBoxLayout *box_e_layout = new QVBoxLayout(box_e_widget);

    QWidget *project_dir_widget = new QWidget();
    QHBoxLayout *project_dir_layout = new QHBoxLayout(project_dir_widget);

    QWidget *project_widget = new QWidget();
    QHBoxLayout *project_layout = new QHBoxLayout(project_widget);

    QWidget *render_node_widget = new QWidget();
    QHBoxLayout *render_node_layout = new QHBoxLayout(render_node_widget);

    QWidget *job_widget = new QWidget();
    QHBoxLayout *job_layout = new QHBoxLayout(job_widget);

    QWidget *frame_range_widget = new QWidget();
    QHBoxLayout *frame_range_layout = new QHBoxLayout(frame_range_widget);

    QWidget *task_size_widget = new QWidget();
    QHBoxLayout *task_size_layout = new QHBoxLayout(task_size_widget);

    QWidget *priority_widget = new QWidget();
    QHBoxLayout *priority_layout = new QHBoxLayout(priority_widget);

    QWidget *server_group_widget = new QWidget();
    QHBoxLayout *server_group_layout = new QHBoxLayout(server_group_widget);

    QWidget *comment_widget = new QWidget();
    QHBoxLayout *comment_layout = new QHBoxLayout(comment_widget);

    project_dir_label = new QLabel("Project Folder:");
    project_label = new QLabel("Project File:");
    render_node_label = new QLabel("Render Node:");
    QLabel *job_name_label = new QLabel("Job Name:");
    QLabel *frame_range_label = new QLabel("Frame Range:");
    QLabel *task_size_label = new QLabel("Task Size:");
    QLabel *server_group_label = new QLabel("Server Group:");
    QLabel *comment_label = new QLabel("Comment:");
    QLabel *priority_label = new QLabel("Priority:");
    ffmpeg_widget =
        new ffmpeg_submit({label_width, h_margin, v_margin, v_padding});

    QScrollArea *scrollArea = new QScrollArea();


    // Layouts Settings
    main_layout->setMargin(0);
    box_a_layout->setMargin(0);
    box_b_layout->setMargin(0);
    box_c_layout->setMargin(0);
    box_d_layout->setMargin(0);
    box_e_layout->setMargin(0);

    box_a_layout->setSpacing(0);
    box_b_layout->setSpacing(0);
    box_c_layout->setSpacing(0);
    box_d_layout->setSpacing(0);
    box_e_layout->setSpacing(0);

    main_layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    project_layout->setContentsMargins(h_margin, v_padding, h_margin, v_margin);
    project_dir_layout->setContentsMargins(h_margin, v_margin, h_margin, v_margin);
    render_node_layout->setContentsMargins(h_margin, v_margin, h_margin, v_padding);

    job_layout->setContentsMargins(h_margin, v_padding, h_margin, v_padding);

    frame_range_layout->setContentsMargins(h_margin, v_padding, h_margin, v_margin);
    task_size_layout->setContentsMargins(h_margin, v_margin, h_margin, v_margin);
    priority_layout->setContentsMargins(h_margin, v_margin, h_margin, v_padding);

    server_group_layout->setContentsMargins(h_margin, v_padding, h_margin, v_margin);
    comment_layout->setContentsMargins(h_margin, v_margin, h_margin, v_padding);

    priority_layout->setAlignment(Qt::AlignLeft);
    server_group_layout->setAlignment(Qt::AlignLeft);


    // Widgets
    box_a_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    box_b_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    box_c_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    box_d_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    box_e_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ffmpeg_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    box_a_widget->setObjectName("box_widget");
    box_b_widget->setObjectName("box_widget");
    box_c_widget->setObjectName("box_widget");
    box_d_widget->setObjectName("box_widget");
    box_e_widget->setObjectName("box_widget");
    ffmpeg_widget->setObjectName("box_widget");

    // Labels
    project_dir_label->setFixedWidth(label_width);
    project_label->setFixedWidth(label_width);
    render_node_label->setFixedWidth(label_width);
    job_name_label->setFixedWidth(label_width);
    frame_range_label->setFixedWidth(label_width);
    task_size_label->setFixedWidth(label_width);
    priority_label->setFixedWidth(label_width);
    server_group_label->setFixedWidth(label_width);
    comment_label->setFixedWidth(label_width);

    project_dir_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    project_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    render_node_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    job_name_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    frame_range_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    task_size_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    priority_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    server_group_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    comment_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);


    // Settings
    QStringList items = {"Maya", "Houdini", "FFmpeg"};
    software_box->add_items(items);

    priority->add_items({"Very High", "High", "Normal", "Low", "Very Low"});
    priority->set_index(2);

    scrollArea->setWidget(main_widget);
    scrollArea->setWidgetResizable(true);

    // Layout

    project_dir_layout->addWidget(project_dir_label);
    project_dir_layout->addWidget(project_dir_edit);
    project_dir_layout->addWidget(project_dir_button);

    project_layout->addWidget(project_label);
    project_layout->addWidget(project_edit);
    project_layout->addWidget(project_button);

    render_node_layout->addWidget(render_node_label);
    render_node_layout->addWidget(render_node_edit);

    job_layout->addWidget(job_name_label);
    job_layout->addWidget(job_name);

    frame_range_layout->addWidget(frame_range_label);
    frame_range_layout->addWidget(first_frame_edit);
    frame_range_layout->addWidget(last_frame_edit);

    task_size_layout->addWidget(task_size_label);
    task_size_layout->addWidget(task_size_edit);

    priority_layout->addWidget(priority_label);
    priority_layout->addWidget(priority);

    server_group_layout->addWidget(server_group_label);
    server_group_layout->addWidget(server_group_box);

    comment_layout->addWidget(comment_label);
    comment_layout->addWidget(comment_edit);

    box_a_layout->addWidget(project_widget);
    box_a_layout->addWidget(project_dir_widget);
    box_a_layout->addWidget(render_node_widget);
    box_b_layout->addWidget(job_widget);
    box_c_layout->addWidget(frame_range_widget);
    box_c_layout->addWidget(task_size_widget);
    box_c_layout->addWidget(priority_widget);
    box_d_layout->addWidget(server_group_widget);
    box_d_layout->addWidget(comment_widget);
    box_e_layout->addWidget(suspend_box);

    main_layout->addWidget(software_box);
    main_layout->addWidget(ffmpeg_widget);
    main_layout->addWidget(box_a_widget);
    main_layout->addWidget(box_b_widget);
    main_layout->addWidget(box_c_widget);
    main_layout->addWidget(box_d_widget);
    main_layout->addWidget(box_e_widget);
    main_layout->addWidget(submit_button);

    layout->addWidget(scrollArea);
}

void submit::connections()
{
    connect(server_group_box, &combo_box::clicked, this,
            &submit::update_server_groups);

    connect(software_box, &combo_box::current_text_changed, this,
            &submit::set_software);

    connect(project_dir_button, &QPushButton::clicked, this, [this]() {
        QString file_path = QFileDialog::getExistingDirectory(
            _monitor, "Project Folder", project_dir_edit->text());

        if (!file_path.isEmpty())
            project_dir_edit->setText(file_path);
    });

    connect(project_button, &QPushButton::clicked, this, [this]() {
        QString file_name = QFileDialog::getOpenFileName(
            _monitor, "Project File", project_edit->text());

        if (not file_name.isEmpty())
            project_edit->setText(file_name);

        update_ffmpeg_panel();
    });

    connect(submit_button, &QPushButton::clicked, this, [this]() {
        submit_start(software_box->get_current_text().toLower());
    });
}

void submit::set_software(QString software)
{
    software = software.toLower();

    ffmpeg_widget->hide();

    project_dir_label->hide();
    project_dir_edit->hide();
    project_dir_button->hide();

    render_node_label->hide();
    render_node_edit->hide();

    if (software == "maya")
    {
        project_dir_label->show();
        project_dir_edit->show();
        project_dir_button->show();

        project_label->setText("Scene File:");
        project_dir_label->setText("Project Folder:");
    }

    else if (software == "houdini")
    {
        render_node_label->show();
        render_node_edit->show();

        render_node_label->setText("Engine:");
        render_node_edit->setText("/out/arnold1");

        project_label->setText("Project:");
    }
    else if (software == "ffmpeg")
    {
        project_dir_label->show();
        project_dir_edit->show();
        project_dir_button->show();
        project_dir_label->setText("Output Folder:");

        render_node_label->show();
        render_node_edit->show();
        render_node_label->setText("Movie Name:");
        render_node_edit->setText("");

        project_label->setText("Input File:");

        ffmpeg_widget->show();
    }

    software_box->set_current_text(software);
}

void submit::update_server_groups()
{
    groups_class *groups = static_cast<groups_class *>(
        static_cast<monitor *>(_monitor)->get_groups_widget());

    server_group_box->clear();
    server_group_box->add_items({"None"});

    for (QString group : groups->get_groups())
        server_group_box->add_items({group});
}

void submit::submit_start(QString software)
{
    QString system = _linux ? "Linux" : "Windows";
    QString misc;

    if (software == "ffmpeg")
    {
        QJsonObject misc_obj = {{"output_dir", project_dir_edit->text()},
                                {"movie_name", render_node_edit->text()},
                                {"command", ffmpeg_widget->get_command()}};
        misc = jots(misc_obj);
    }

    QJsonArray info = {job_name->text(),
                       "",
                       server_group_box->get_current_text(),
                       first_frame_edit->text().toInt(),
                       last_frame_edit->text().toInt(),
                       task_size_edit->text().toInt(),
                       priority->get_current_text(),
                       suspend_box->isChecked(),
                       comment_edit->text(),
                       software,
                       project_edit->text(),
                       misc,
                       system,
                       1,
                       render_node_edit->text()};

    bool ok = true;
    QString details = "Incomplete Fiels:\n";

    if (job_name->text().isEmpty())
    {
        details += "Job Name\n";
        ok = false;
    }
    if (first_frame_edit->text().isEmpty())
    {
        details += "First Frame\n";
        ok = false;
    }
    if (last_frame_edit->text().isEmpty())
    {
        details += "Last Frame\n";
        ok = false;
    }
    if (task_size_edit->text().isEmpty())
    {
        details += "Task Size\n";
        ok = false;
    }

    QMessageBox *msg = new QMessageBox(this);
    msg->setWindowTitle("Submit Information");

    if (ok)
    {
        send_job(info);
        msg->setText("The " + job_name->text() + " job has sended.");
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
    QJsonObject panel = {{"software", software_box->get_current_text()},
                         {"ffmpeg_presets", ffmpeg_widget->get_preset()},
                         {"project_dir", project_dir_edit->text()},
                         {"project", project_edit->text()},
                         {"render_node", render_node_edit->text()},
                         {"job_name", job_name->text()},
                         {"first_frame", first_frame_edit->text()},
                         {"last_frame", last_frame_edit->text()},
                         {"task_size", task_size_edit->text()},
                         {"priority", priority->get_current_text()},
                         {"server_group", server_group_box->get_current_text()},
                         {"comment", comment_edit->text()},
                         {"suspend", suspend_box->isChecked()}};

    jwrite(VINARENDER_CONF_PATH + "/submit_panel.json", panel);
}

void submit::panel_open()
{
    QJsonObject panel = jread(VINARENDER_CONF_PATH + "/submit_panel.json");

    software_box->set_current_text(panel["software"].toString(), true);
    ffmpeg_widget->set_preset(panel["ffmpeg_presets"].toString());

    project_dir_edit->setText(panel["project_dir"].toString());
    project_edit->setText(panel["project"].toString());
    render_node_edit->setText(panel["render_node"].toString());

    job_name->setText(panel["job_name"].toString());

    first_frame_edit->setText(panel["first_frame"].toString());
    last_frame_edit->setText(panel["last_frame"].toString());
    task_size_edit->setText(panel["task_size"].toString());

    priority->set_current_text(panel["priority"].toString());
    server_group_box->set_current_text(panel["server_group"].toString());

    comment_edit->setText(panel["comment"].toString());
    suspend_box->setChecked(panel["suspend"].toBool());
}

void submit::update_ffmpeg_panel()
{
    if (software_box->get_current_text().toLower() != "ffmpeg")
        return;

    QString src_movie = project_edit->text();
    project_dir_edit->setText(os::dirname(src_movie));

    QString movie_name = path_util::basename_no_ext(src_movie);
    render_node_edit->setText(movie_name + "_output");

    job_name->setText(movie_name);

    int first_frame, last_frame, task_size;
    calc_ffmpeg_data(src_movie, &first_frame, &last_frame, &task_size);

    first_frame_edit->setText(QString::number(first_frame));
    last_frame_edit->setText(QString::number(last_frame));
    task_size_edit->setText(QString::number(task_size));
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
