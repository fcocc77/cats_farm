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
#include "tcp.h"

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
}

submit::~submit() {}

void submit::ui()
{
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

    int h_margin = 20;
    int v_margin = 3;
    int v_padding = 15;

    project_dir_layout->setContentsMargins(h_margin, v_padding, h_margin, v_margin);
    project_layout->setContentsMargins(h_margin, v_margin, h_margin, v_margin);
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

    box_a_widget->setObjectName("box_widget");
    box_b_widget->setObjectName("box_widget");
    box_c_widget->setObjectName("box_widget");
    box_d_widget->setObjectName("box_widget");
    box_e_widget->setObjectName("box_widget");

    // Labels
    int label_width = 100;
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
    QStringList items = {"Maya", "Houdini", "FFMpeg"};
    software_box->add_items(items);

    priority->add_items({"Very High", "High", "Normal", "Low", "Very Low"});
    priority->set_current_index(2);

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

    box_a_layout->addWidget(project_dir_widget);
    box_a_layout->addWidget(project_widget);
    box_a_layout->addWidget(render_node_widget);
    box_b_layout->addWidget(job_widget);
    box_c_layout->addWidget(frame_range_widget);
    box_c_layout->addWidget(task_size_widget);
    box_c_layout->addWidget(priority_widget);
    box_d_layout->addWidget(server_group_widget);
    box_d_layout->addWidget(comment_widget);
    box_e_layout->addWidget(suspend_box);

    main_layout->addWidget(software_box);
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
            &submit::update_software_box);

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
    });

    connect(submit_button, &QPushButton::clicked, this,
            [this]() { submit_start(software_box->get_current_text()); });
}

void submit::update_software_box(QString software)
{
    if (software == "Maya")
    {
        project_dir_edit->setDisabled(0);
        project_dir_button->setDisabled(0);
        render_node_edit->setDisabled(1);

        project_dir_label->setText("Project Folder:");
        render_node_label->setText("...");
        render_node_edit->setText("");
    }

    else if (software == "Houdini")
    {
        project_dir_edit->setDisabled(1);
        project_dir_button->setDisabled(1);
        render_node_edit->setDisabled(0);

        project_dir_label->setText("...");
        render_node_label->setText("Engine:");
        render_node_edit->setText("/out/arnold1");
        project_dir_edit->setText("");
    }
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

    QJsonArray info = {job_name->text(), "", server_group_box->get_current_text(),
                       first_frame_edit->text().toInt(),
                       last_frame_edit->text().toInt(),
                       task_size_edit->text().toInt(), priority->get_current_text(),
                       suspend_box->isChecked(), comment_edit->text(), software,
                       // project,
                       // extra,
                       system,
                       // instances.toInt(),
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
        QJsonObject settings = jread(VINARENDER_CONF_PATH + "/settings.json");
        int port = settings["manager"].toObject()["port"].toInt();
        QString host = settings["current_manager"].toString();

        tcpClient(host, port, jats({4, info}));

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
