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

    this->setObjectName("submit_widget");

    software_box = new combo_box();

    project_dir_edit = new QLineEdit();
    project_dir_button = new QPushButton("Open");
    project_dir_label = new QLabel("Project Folder:");

    project_button = new QPushButton("Open");
    project_edit = new QLineEdit();
    project_label = new QLabel("Project File:");

    render_node_edit = new QLineEdit();
    render_node_label = new QLabel("Render Node:");

    job_name = new QLineEdit();
    server_group_box = new combo_box();
    priority = new QComboBox();
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
    QVBoxLayout *vboxSubmit = new QVBoxLayout();
    vboxSubmit->setContentsMargins(15, 15, 15, 15);

    QStringList items = {"Maya", "Houdini"};
    software_box->addItems(items);
    vboxSubmit->addWidget(software_box);

    // box 1
    QVBoxLayout *vbox2 = new QVBoxLayout();
    vbox2->setContentsMargins(15, 15, 15, 15);

    QHBoxLayout *hbox7 = new QHBoxLayout();
    hbox7->setContentsMargins(0, 0, 0, 0);
    project_dir_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    project_dir_label->setMinimumWidth(70);
    project_dir_button->setMinimumWidth(70);
    //------------------------------
    hbox7->addWidget(project_dir_label);
    hbox7->addWidget(project_dir_edit);
    hbox7->addWidget(project_dir_button);
    QWidget *widget7 = new QWidget();
    widget7->setLayout(hbox7);
    widget7->setObjectName("style2");
    vbox2->addWidget(widget7);

    QHBoxLayout *hbox8 = new QHBoxLayout();
    hbox8->setContentsMargins(0, 0, 0, 0);
    project_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    project_label->setMinimumWidth(70);
    project_button->setMinimumWidth(70);
    //------------------------------
    hbox8->addWidget(project_label);
    hbox8->addWidget(project_edit);
    hbox8->addWidget(project_button);
    QWidget *widget8 = new QWidget();
    widget8->setLayout(hbox8);
    widget8->setObjectName("style2");
    vbox2->addWidget(widget8);

    QHBoxLayout *hbox85 = new QHBoxLayout();
    hbox85->setContentsMargins(0, 0, 0, 0);
    //------------------------------
    QWidget *widget85 = new QWidget();
    widget85->setLayout(hbox85);
    widget85->setObjectName("style2");
    vbox2->addWidget(widget85);

    QHBoxLayout *hbox9 = new QHBoxLayout();
    hbox9->setContentsMargins(0, 0, 0, 0);
    render_node_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    render_node_label->setMinimumWidth(70);
    //------------------------------
    hbox9->addWidget(render_node_label);
    hbox9->addWidget(render_node_edit);
    QWidget *widget9 = new QWidget();
    widget9->setLayout(hbox9);
    widget9->setObjectName("style2");
    vbox2->addWidget(widget9);

    QWidget *widget2 = new QWidget();
    widget2->setLayout(vbox2);
    widget2->setObjectName("style1");

    vboxSubmit->addWidget(widget2);
    //---------------------------------------

    // box 2
    QVBoxLayout *vbox3 = new QVBoxLayout();
    vbox3->setContentsMargins(15, 15, 15, 15);
    QHBoxLayout *hbox10 = new QHBoxLayout();
    hbox10->setContentsMargins(0, 0, 0, 0);
    QLabel *labelJobName = new QLabel("Job Name:");
    labelJobName->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    labelJobName->setMinimumWidth(70);
    //---------------------------------------
    hbox10->addWidget(labelJobName);
    hbox10->addWidget(job_name);
    QWidget *widget10 = new QWidget();
    widget10->setLayout(hbox10);
    widget10->setObjectName("style2");

    vbox3->addWidget(widget10);

    QWidget *widget3 = new QWidget();
    widget3->setLayout(vbox3);
    widget3->setObjectName("style1");

    vboxSubmit->addWidget(widget3);
    //---------------------------------------

    // box 3
    QVBoxLayout *vbox4 = new QVBoxLayout();
    vbox4->setContentsMargins(15, 15, 15, 15);

    QHBoxLayout *hbox11 = new QHBoxLayout();
    hbox11->setContentsMargins(0, 0, 0, 0);

    QLabel *labelFR = new QLabel("Frame Range:");
    labelFR->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    labelFR->setMinimumWidth(70);

    hbox11->addWidget(labelFR);
    hbox11->addWidget(first_frame_edit);
    hbox11->addWidget(last_frame_edit);
    QWidget *widget11 = new QWidget();
    widget11->setLayout(hbox11);
    widget11->setObjectName("style2");
    vbox4->addWidget(widget11);

    QHBoxLayout *hbox12 = new QHBoxLayout();
    hbox12->setContentsMargins(0, 0, 0, 0);

    QLabel *labelTS = new QLabel("Task Size:");
    labelTS->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    labelTS->setMinimumWidth(70);

    hbox12->addWidget(labelTS);
    hbox12->addWidget(task_size_edit);
    QWidget *widget12 = new QWidget();
    widget12->setLayout(hbox12);
    widget12->setObjectName("style2");
    vbox4->addWidget(widget12);

    QHBoxLayout *hbox13 = new QHBoxLayout();
    hbox13->setContentsMargins(0, 0, 0, 0);

    QLabel *labelPriority = new QLabel("           Priority:");
    labelPriority->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    labelPriority->setMaximumWidth(70);

    priority->addItems({"Very High", "High", "Normal", "Low", "Very Low"});
    priority->setCurrentIndex(2);
    priority->setMaximumWidth(80);

    hbox13->addWidget(labelPriority);
    hbox13->addWidget(priority);
    QLabel *null = new QLabel(" ");
    hbox13->addWidget(null);
    QWidget *widget13 = new QWidget();
    widget13->setLayout(hbox13);
    widget13->setObjectName("style2");
    vbox4->addWidget(widget13);

    QWidget *widget4 = new QWidget();
    widget4->setLayout(vbox4);
    widget4->setObjectName("style1");

    vboxSubmit->addWidget(widget4);
    //---------------------------------------

    // box 4
    QVBoxLayout *vbox5 = new QVBoxLayout();
    vbox5->setContentsMargins(15, 15, 15, 15);
    QHBoxLayout *hbox14 = new QHBoxLayout();
    hbox14->setContentsMargins(0, 0, 0, 0);
    QLabel *labelSG = new QLabel("Server Group:");
    labelSG->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    labelSG->setMinimumWidth(70);

    QLabel *labelServer = new QLabel("Server:");
    labelServer->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    hbox14->addWidget(labelSG);
    hbox14->addWidget(server_group_box);
    hbox14->addWidget(labelServer);
    QWidget *widget14 = new QWidget();
    widget14->setLayout(hbox14);
    widget14->setObjectName("style2");
    vbox5->addWidget(widget14);

    QHBoxLayout *hbox16 = new QHBoxLayout();
    hbox16->setContentsMargins(0, 0, 0, 0);
    QLabel *labelComment = new QLabel("Comment:");
    labelComment->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    labelComment->setMinimumWidth(70);
    hbox16->addWidget(labelComment);
    hbox16->addWidget(comment_edit);
    QWidget *widget16 = new QWidget();
    widget16->setLayout(hbox16);
    widget16->setObjectName("style2");
    vbox5->addWidget(widget16);
    QWidget *widget5 = new QWidget();
    widget5->setLayout(vbox5);
    widget5->setObjectName("style1");
    vboxSubmit->addWidget(widget5);

    // box 5
    QVBoxLayout *vbox6 = new QVBoxLayout();
    vbox6->setContentsMargins(15, 15, 15, 15);
    vbox6->addWidget(suspend_box);

    QWidget *widget6 = new QWidget();
    widget6->setLayout(vbox6);
    widget6->setObjectName("style1");
    vboxSubmit->addWidget(widget6);

    QWidget *widget1 = new QWidget();
    widget1->setLayout(vboxSubmit);
    widget1->setMaximumHeight(590);

    vboxSubmit->addWidget(submit_button);

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(widget1);
    scrollArea->setWidgetResizable(true);
    layout->addWidget(scrollArea);
}

void submit::connections()
{
    connect(server_group_box, &combo_box::clicked, this,
            &submit::update_server_groups);

    connect(software_box, &combo_box::currentTextChanged, this,
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
            [this]() { submit_start(software_box->currentText()); });
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
    server_group_box->addItems({"None"});

    for (QString group : groups->get_groups())
        server_group_box->addItems({group});
}

void submit::submit_start(QString software)
{
    QString system = _linux ? "Linux" : "Windows";

    QJsonArray info = {job_name->text(), "", server_group_box->currentText(),
                       first_frame_edit->text().toInt(),
                       last_frame_edit->text().toInt(),
                       task_size_edit->text().toInt(), priority->currentIndex(),
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
        QJsonObject settings = jread(VINARENDER_PATH + "/etc/settings.json");
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
