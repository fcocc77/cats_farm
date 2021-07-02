#include <QLabel>
#include <QVBoxLayout>

#include "options.h"
#include "util.h"
#include "combo_box.h"
#include "main_window.h"
#include "properties.h"

options_class::options_class(QWidget *__monitor)
    : _monitor(__monitor)
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

    QWidget *box_dialog = new QWidget();
    QHBoxLayout *box_dialog_layout = new QHBoxLayout(box_dialog);

    QPushButton *ok_button = new QPushButton("Ok");
    QPushButton *cancel_button = new QPushButton("Cancel");

    box_dialog_layout->addWidget(ok_button);
    box_dialog_layout->addWidget(cancel_button);

    main_layout->addWidget(_time_knobs);
    main_layout->addWidget(_misc_knobs);
    main_layout->addWidget(box_dialog);

    // Connecciones

    // connect(job_modify_action, &QAction::triggered, this, &jobs_class::modify);
    // job_modify_action->setShortcut(QString("M"));

    connect(ok_button, &QPushButton::clicked, this, &options_class::options_ok);

    // connect(cancel_button, &QPushButton::clicked,
            // [this]() { properties->parentWidget()->hide(); });
}

void options_class::update_panel()
{
    if (!isVisible())
        return;

    print("update panel");
}

void options_class::options_ok()
{
    // QJsonArray machines = {};
    // // guarda el grupo del combobox en el array
    // QJsonArray group;
    // QString current_group = options->group_combobox->currentText();
    // group.push_back(current_group);

    // int priority = options->priority_combobox->currentIndex();
    // int first_frame = options->first_frame_edit->text().toInt();
    // int last_frame = options->last_frame_edit->text().toInt();
    // int task_size = options->task_size_edit->text().toInt();
    // QString comment = options->comment_edit->text();
    // QString _job_name = options->job_name_edit->text();

    // int instances = options->instances_edit->text().toInt();
    // if (instances > 16)
        // instances = 16;

    // QJsonArray pks;
    // auto selected = this->selectedItems();
    // selected.push_front(first_job_item);

    // QStringList repeatItem;
    // for (auto item : selected)
    // {
        // QString job_name = item->text(0);
        // QJsonArray options = {machines,   group,     priority,
                              // comment,    instances, first_frame,
                              // last_frame, task_size, _job_name};
        // // el primer item se repite asi que si esta 2 veces no lo agrega otra
        // // vez
        // if (not repeatItem.contains(job_name))
            // pks.push_back({{job_name, options, "write"}});
        // repeatItem.push_back(job_name);
    // }
    // pks = {"jobOptions", pks};

    // QString ask = "Sure you want to send the changes? \nSome frames will be "
                  // "lost due to the size of the tasks.";

    // QMessageBox::StandardButton reply;
    // reply = QMessageBox::question(monitor, "Job Options", ask,
                                  // QMessageBox::Yes | QMessageBox::No);
    // if (reply == QMessageBox::Yes)
    // {
        // tcpClient(shared->manager_host, shared->manager_port, jats({3, pks}));
        // properties->parentWidget()->hide();
    // }
}

void options_class::open_panel()
{
    properties_class *properties =
        static_cast<monitor *>(_monitor)->get_properties();

    properties->switch_widget("options");

    // // Recive los servers del jobs que estan en el manager
    // auto selected = this->selectedItems();
    // first_job_item = selected[0];

    // if (not selected.empty())
    // {
        // QString job_name = selected.takeLast()->text(0);

        // QJsonArray send = {QJsonArray({job_name, "options", "read"})};
        // send = {"jobOptions", send};
        // QString recv = tcpClient(shared->manager_host, shared->manager_port,
                                 // jats({3, send}));
        // QJsonArray pks = jafs(recv);

        // QStringList serverGroupExist;
        // for (QJsonValue sg : pks[1].toArray())
            // serverGroupExist.push_back(sg.toString());

        // int priority = pks[2].toInt();
        // QString comment = pks[3].toString();
        // int instances = pks[4].toInt();
        // int task_size = pks[5].toInt();
        // QString _job_name = pks[6].toString();
        // int first_frame = pks[7].toInt();
        // int last_frame = pks[8].toInt();

        // options->priority_combobox->setCurrentIndex(priority);
        // options->first_frame_edit->setText(QString::number(first_frame));
        // options->last_frame_edit->setText(QString::number(last_frame));
        // options->job_name_edit->setText(_job_name);
        // options->task_size_edit->setText(QString::number(task_size));
        // options->comment_edit->setText(comment);
        // options->instances_edit->setText(QString::number(instances));

        // // Agrega grupos al combobox y establese el item correspondiente
        // options->group_combobox->clear();
        // options->group_combobox->addItem("None");
        // QString current_group = "None";
        // for (int i = 0; i < groups->topLevelItemCount(); ++i)
        // {
            // QString name = groups->topLevelItem(i)->text(2);
            // options->group_combobox->addItem(name);

            // if (serverGroupExist.contains(name))
                // current_group = name;
        // }
        // options->group_combobox->setCurrentText(current_group);
    // }
}
