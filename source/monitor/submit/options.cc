#include <QVBoxLayout>

#include "options.h"
#include "util.h"
#include "combo_box.h"
#include "main_window.h"
#include "properties.h"

options_class::options_class(QWidget *__monitor, shared_variables *_shared)
    : _monitor(__monitor)
    , shared(_shared)
{
    setup_ui();
}

options_class::~options_class() {}

void options_class::setup_ui()
{
    this->setObjectName("options");

    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->setMargin(0);

    _time_knobs = new time_knobs();
    _misc_knobs = new misc_knobs(_monitor);

    selected_jobs_label = new QLabel("selected jobs");

    box_dialog = new QWidget();
    QHBoxLayout *box_dialog_layout = new QHBoxLayout(box_dialog);

    QPushButton *save_button = new QPushButton("Save");
    QPushButton *close_button = new QPushButton("Close");

    box_dialog_layout->addWidget(save_button);
    box_dialog_layout->addWidget(close_button);

    main_layout->addWidget(selected_jobs_label);
    main_layout->addWidget(_time_knobs);
    main_layout->addWidget(_misc_knobs);
    main_layout->addWidget(box_dialog);

    // Connecciones

    // connect(job_modify_action, &QAction::triggered, this, &jobs_class::modify);
    // job_modify_action->setShortcut(QString("M"));

    connect(save_button, &QPushButton::clicked, this, &options_class::save);

    connect(close_button, &QPushButton::clicked, [this]() {
        static_cast<monitor *>(_monitor)->get_toolbar()->hide_properties();
    });
}

void options_class::update_panel(bool clear)
{
    if (!isVisible())
        return;

    jobs_class *jobs = static_cast<monitor *>(_monitor)->get_jobs_widget();

    int count = jobs->selectedItems().count();
    selected_jobs.clear();

    for (QTreeWidgetItem *item : jobs->selectedItems())
        selected_jobs.push_back(item->text(0));

    QString selected_items;
    for (QString job_name : selected_jobs)
        selected_items += job_name + "  -   ";

    if (clear)
    {
        selected_items = "";
        count = 0;
        set_disabled_all(true);
    }

    selected_jobs_label->setText(selected_items + "\n" +
                                 QString::number(count) + " Jobs Selected");

    if (count == 1)
        uptate_panel_from_job(jobs->selectedItems()[0]->text(0));
}

void options_class::set_disabled_all(bool disable)
{
    _time_knobs->clear();
    _time_knobs->set_disabled(disable);

    box_dialog->setDisabled(disable);

    _misc_knobs->clear();
    _misc_knobs->set_disabled(disable);
}

void options_class::uptate_panel_from_job(QString job_name)
{
    set_disabled_all(false);

    QJsonObject options = {{"job_name", job_name}, {"action", "read"}};
    QJsonArray send = {"job_options", options};

    QString response =
        tcpClient(shared->manager_host, shared->manager_port, jats({3, send}));

    QJsonObject _response = jofs(response);

    _time_knobs->set_first_frame(_response["first_frame"].toInt());
    _time_knobs->set_last_frame(_response["last_frame"].toInt());
    _time_knobs->set_task_size(_response["task_size"].toInt());

    _misc_knobs->set_instances(_response["instances"].toInt());
    _misc_knobs->set_job_name(_response["job_name"].toString());
    _misc_knobs->set_comment(_response["comment"].toString());
    _misc_knobs->set_server_group(_response["server_group"].toString());
    _misc_knobs->set_priority(_response["priority"].toInt());
}

void options_class::save()
{

    QString ask = "Sure you want to send the changes? \nSome frames will be "
                  "lost due to the size of the tasks.";

    // QMessageBox::StandardButton reply;
    // reply = QMessageBox::question(_monitor, "Job Options", ask,
                                  // QMessageBox::Yes | QMessageBox::No);

    // if (reply == QMessageBox::No)
        // return;

    for (QString job_name : selected_jobs)
    {
        QJsonObject options = {
            {"comment", _misc_knobs->get_comment()},
            {"instances", _misc_knobs->get_instances()},
            {"job_name", _misc_knobs->get_job_name()},
            {"first_frame", _time_knobs->get_first_frame()},
            {"last_frame", _time_knobs->get_last_frame()},
            {"task_size", _time_knobs->get_task_size()},
            {"priority", _misc_knobs->get_priority()},
            {"server_group", _misc_knobs->get_server_group()}};

        QJsonObject data = {
            {"job_name", job_name}, {"action", "write"}, {"options", options}};

        QJsonArray send = {"job_options", data};

        tcpClient(shared->manager_host, shared->manager_port, jats({3, send}));
    }
}

void options_class::open_panel()
{
    properties_class *properties =
        static_cast<monitor *>(_monitor)->get_properties();

    properties->switch_widget("options");
}
