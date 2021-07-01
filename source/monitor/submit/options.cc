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

    QWidget *box_a_widget = new QWidget();
    QWidget *box_b_widget = new QWidget();
    QWidget *box_c_widget = new QWidget();
    QWidget *box_dialog = new QWidget();

    QVBoxLayout *box_a_layout = new QVBoxLayout(box_a_widget);
    QVBoxLayout *box_b_layout = new QVBoxLayout(box_b_widget);
    QVBoxLayout *box_c_layout = new QVBoxLayout(box_c_widget);
    QHBoxLayout *box_dialog_layout = new QHBoxLayout(box_dialog);

    QWidget *job_name_widget = new QWidget();
    QWidget *priority_widget = new QWidget();
    QWidget *frame_range_widget = new QWidget();
    QWidget *task_size_widget = new QWidget();
    QWidget *instances_widget = new QWidget();
    QWidget *comment_widget = new QWidget();

    QHBoxLayout *job_name_layout = new QHBoxLayout(job_name_widget);
    QHBoxLayout *priority_layout = new QHBoxLayout(priority_widget);
    QHBoxLayout *frame_range_layout = new QHBoxLayout(frame_range_widget);
    QHBoxLayout *task_size_layout = new QHBoxLayout(task_size_widget);
    QHBoxLayout *instances_layout = new QHBoxLayout(instances_widget);
    QHBoxLayout *comment_layout = new QHBoxLayout(comment_widget);

    QLabel *job_name_label = new QLabel("Job Name:");
    QLabel *priority_label = new QLabel("Priority:");
    QLabel *server_group_label = new QLabel("Server Group:");
    QLabel *frame_range_label = new QLabel("Frame Range:");
    QLabel *task_size_label = new QLabel("Task Size:");
    QLabel *instances_label = new QLabel("Instances:");
    QLabel *comment_label = new QLabel("Comment:");

    QLineEdit *job_name_edit = new QLineEdit();
    QLineEdit *first_frame_edit = new QLineEdit();
    QLineEdit *last_frame_edit = new QLineEdit();
    QLineEdit *task_size_edit = new QLineEdit();
    QLineEdit *instances_edit = new QLineEdit();
    QLineEdit *comment_edit = new QLineEdit();

    combo_box *priority_box = new combo_box();
    combo_box *server_group_box = new combo_box();

    QPushButton *ok_button = new QPushButton("Ok");
    QPushButton *cancel_button = new QPushButton("Cancel");

    // Ajustes
    priority_box->add_items({"Very High", "High", "Normal", "Low", "Very Low"});

    box_a_widget->setObjectName("box_widget");
    box_b_widget->setObjectName("box_widget");
    box_c_widget->setObjectName("box_widget");
    box_dialog->setObjectName("box_widget");

    // Layout
    job_name_layout->addWidget(job_name_label);
    job_name_layout->addWidget(job_name_edit);

    priority_layout->addWidget(priority_label);
    priority_layout->addWidget(priority_box);
    priority_layout->addWidget(server_group_label);
    priority_layout->addWidget(server_group_box);

    frame_range_layout->addWidget(frame_range_label);
    frame_range_layout->addWidget(first_frame_edit);
    frame_range_layout->addWidget(last_frame_edit);

    task_size_layout->addWidget(task_size_label);
    task_size_layout->addWidget(task_size_edit);

    instances_layout->addWidget(instances_label);
    instances_layout->addWidget(instances_edit);

    comment_layout->addWidget(comment_label);
    comment_layout->addWidget(comment_edit);

    box_a_layout->addWidget(job_name_widget);
    box_a_layout->addWidget(priority_widget);

    box_b_layout->addWidget(frame_range_widget);
    box_b_layout->addWidget(task_size_widget);
    box_b_layout->addWidget(instances_widget);

    box_c_layout->addWidget(comment_widget);

    box_dialog_layout->addWidget(ok_button);
    box_dialog_layout->addWidget(cancel_button);

    main_layout->addWidget(_time_knobs);
    main_layout->addWidget(box_a_widget);
    main_layout->addWidget(box_b_widget);
    main_layout->addWidget(box_c_widget);
    main_layout->addWidget(box_dialog);

    // QLayout *first_knobs = qlayout(main_layout, "v", "first_knobs");
    // QLayout *name_knobs = qlayout(first_knobs, "h");
    // QLabel *name_label = new QLabel("Job Name:");

    // job_name_edit = new QLineEdit();
    // QLayout *priority_knobs = qlayout(first_knobs, "h");
    // QLabel *priority_label = new QLabel("Priority:");

    // priority_combobox = new QComboBox();
    // priority_combobox->addItem("Very High");
    // priority_combobox->addItem("High");
    // priority_combobox->addItem("Normal");
    // priority_combobox->addItem("Low");
    // priority_combobox->addItem("Very Low");

    // QLabel *group_label = new QLabel("Server Group:");

    // group_combobox = new QComboBox();

    // QLayout *second_knobs = qlayout(main_layout, "v", "second_knobs");
    // QLayout *range_knobs = qlayout(second_knobs, "h");
    // QLabel *range_label = new QLabel("Frame Range:");

    // first_frame_edit = new QLineEdit();

    // last_frame_edit = new QLineEdit();
    // QLayout *task_knobs = qlayout(second_knobs, "h");
    // QLabel *task_size_label = new QLabel("Task Size:");

    // task_size_edit = new QLineEdit();
    // QLayout *instances_knobs = qlayout(second_knobs, "h");
    // QLabel *instances_label = new QLabel("Instances:");

    // instances_edit = new QLineEdit();

    // QLayout *third_knobs = qlayout(main_layout, "v", "third_knobs");
    // QLayout *comment_knobs = qlayout(third_knobs, "h");
    // QLabel *comment_label = new QLabel("Comment:");

    // comment_edit = new QLineEdit();

    // QLayout *fourth_knobs = qlayout(main_layout, "v");
    // QLayout *button_knobs = qlayout(fourth_knobs, "h");
    // cancel_button = new QPushButton("Cancel");

    // ok_button = new QPushButton("OK");

    // name_knobs->addWidget(name_label);
    // name_knobs->addWidget(job_name_edit);
    // priority_knobs->addWidget(priority_label);
    // priority_knobs->addWidget(priority_combobox);
    // priority_knobs->addWidget(group_label);
    // priority_knobs->addWidget(group_combobox);
    // range_knobs->addWidget(range_label);
    // range_knobs->addWidget(first_frame_edit);
    // range_knobs->addWidget(last_frame_edit);
    // task_knobs->addWidget(task_size_label);
    // task_knobs->addWidget(task_size_edit);
    // instances_knobs->addWidget(instances_label);
    // instances_knobs->addWidget(instances_edit);
    // comment_knobs->addWidget(comment_label);
    // comment_knobs->addWidget(comment_edit);
    // button_knobs->addWidget(cancel_button);

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
