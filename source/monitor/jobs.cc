#include <jobs.h>

#include <QPixmap>
#include <QtSvg>

jobs_class::jobs_class(
    shared_variables *_shared,
    QMainWindow *_monitor,
    log_class *_log,
    servers_class *_servers,
    options_class *_options,
    groups_class *_groups,
    properties_class *_properties)

    : shared(_shared),
      monitor(_monitor),
      log(_log),
      servers(_servers),
      options(_options),
      groups(_groups),
      properties(_properties)
{
    // Job Acciones
    delete_action = new QAction("Delete");
    job_suspend_action = new QAction("Suspend");
    job_resume_action = new QAction("Resume");
    job_restart_action = new QAction("Restart");
    job_unlock_servers_action = new QAction("Enable blocked servers");
    job_log_action = new QAction("Show Log");
    job_modify_action = new QAction("Modify");
    //------------------------------------------------

    setup_ui();
    connections();
}

jobs_class::~jobs_class()
{
}

void jobs_class::setup_ui()
{
    this->setObjectName("jobs");
    this->setColumnCount(10);
    QStringList columns{"Job Name", "Priority", "Software", "Task Progress", "Status",
                        "Submit Data/Time", "Finished Data/Time", "Estimated Time", "Total Render Time", "Errors"};

    this->setHeaderLabels(columns);

    this->setSelectionMode(QAbstractItemView::ExtendedSelection); // multi seleccion
    this->setAlternatingRowColors(true);                          // item con color alternativos
    this->setIndentation(0);                                      // elimina el margen del principio

    this->setColumnWidth(0, 200); // ajusta el largo de las columnas
    this->setColumnWidth(1, 80);
    this->setColumnWidth(2, 100);
    this->setColumnWidth(3, 200);
    this->setColumnWidth(4, 100);
    this->setColumnWidth(5, 150);
    this->setColumnWidth(6, 150);
    this->setColumnWidth(7, 150);
    this->setColumnWidth(8, 150);

    this->setContextMenuPolicy(Qt::CustomContextMenu);

    this->setSortingEnabled(true);
    this->setFocusPolicy(Qt::NoFocus);
    this->sortByColumn(5, Qt::AscendingOrder);
}

void jobs_class::connections()
{
    connect(this, &QTreeWidget::itemDoubleClicked, this, &jobs_class::modify);
    connect(this, &QTreeWidget::customContextMenuRequested, this, &jobs_class::popup);

    // Job Acciones
    connect(delete_action, &QAction::triggered, this, &jobs_class::item_delete);

    delete_action->setShortcut(QString("Del"));

    connect(job_suspend_action, &QAction::triggered, this, [this]() {
        QString ask = "Sure you want to suspend the job?";
        QString tile = "Job Suspend";
        QString action = "suspend";
        message(&jobs_class::to_action, action, ask, tile, this);
    });

    connect(job_restart_action, &QAction::triggered, this, [this]() {
        QString ask = "Sure you want to restart the job?";
        QString tile = "Job Restart";
        QString action = "restart";
        message(&jobs_class::to_action, action, ask, tile, this);
    });

    connect(job_resume_action, &QAction::triggered, this, [this]() {
        to_action("resume");
    });

    connect(job_unlock_servers_action, &QAction::triggered, this, [this]() {
        to_action("unlock");
    });

    connect(job_log_action, &QAction::triggered, this, &jobs_class::show_log);
    job_log_action->setShortcut(QString("L"));

    connect(job_modify_action, &QAction::triggered, this, &jobs_class::modify);
    job_modify_action->setShortcut(QString("M"));

    connect(options->ok_button, &QPushButton::clicked, this, &jobs_class::options_ok);
    connect(options->cancel_button, &QPushButton::clicked, [this]() {
        properties->parentWidget()->hide();
    });
    //-----------------------------------------------------------------------
}

void jobs_class::popup()
{
    auto selected = this->selectedItems();
    if (not selected.empty())
    {
        QMenu *menu = new QMenu(monitor);

        menu->addAction(job_suspend_action);
        menu->addAction(job_resume_action);
        menu->addAction(delete_action);
        menu->addAction(job_restart_action);

        menu->addSeparator();
        menu->addAction(job_log_action);
        menu->addAction(job_unlock_servers_action);

        menu->addSeparator();
        menu->addAction(job_modify_action);

        menu->popup(QCursor::pos());
    }
}

void jobs_class::show_log()
{

    auto selected = this->selectedItems();
    if (not selected.empty())
    {

        QString job_name = selected[0]->text(0);

        // encuentra job seleccionado en la jobs recividos
        QJsonObject job;
        for (QJsonValue j : shared->jobs)
        {
            QJsonObject _job = j.toObject();
            if (_job["name"].toString() == job_name)
                job = _job;
        }
        //--------------------------------------

        for (QJsonValue t : job["task"].toArray())
        {
            QJsonObject task = t.toObject();
            log_server = task["server"].toString();
            log_server = log_server.split(":")[0];

            if (not(log_server == "..."))
            {

                QStringList vetoed_host;
                QStringList vetoed_name;
                QStringList all_host;
                QStringList all_name;

                for (int i = 0; i < servers->topLevelItemCount(); ++i)
                {
                    auto item = servers->topLevelItem(i);
                    QString name = item->text(0);
                    QString _host = item->text(7);

                    for (QJsonValue vs : job["vetoed_servers"].toArray())
                    {
                        if (name == vs.toString())
                        {
                            vetoed_host.push_back(_host);
                            vetoed_name.push_back(name);
                        }
                    }
                    if (name == log_server)
                    {
                        all_host.push_back(_host);
                        all_name.push_back(name);
                    }
                }

                QString _name;
                QString _host;
                bool failed;
                if (not all_host.empty())
                {
                    _host = all_host[0];
                    _name = all_name[0];
                    failed = false;
                }
                if (not vetoed_host.empty())
                {
                    _host = vetoed_host[0];
                    _name = vetoed_name[0];
                    failed = true;
                }

                QJsonArray send = {_host, QJsonArray({1, failed})};
                QString result = tcpClient(shared->manager_host, shared->manager_port, jats({5, send}));

                log->code_editor->setPlainText(_name + " Log:\n\n" + result);
                break;
            }
            else
                log->code_editor->setPlainText("The jobs has not yet rendered");
        }

        properties->switch_widget("log");
    }
}

void jobs_class::modify()
{
    properties->switch_widget("options");

    // Recive los servers del jobs que estan en el manager
    auto selected = this->selectedItems();
    first_job_item = selected[0];

    if (not selected.empty())
    {
        QString job_name = selected.takeLast()->text(0);

        QJsonArray send = {QJsonArray({job_name, "options", "read"})};
        send = {"jobOptions", send};
        QString recv = tcpClient(shared->manager_host, shared->manager_port, jats({3, send}));
        QJsonArray pks = jafs(recv);
        //-----------------------------------------

        QStringList serverGroupExist;
        for (QJsonValue sg : pks[1].toArray())
            serverGroupExist.push_back(sg.toString());

        int priority = pks[2].toInt();
        QString comment = pks[3].toString();
        int instances = pks[4].toInt();
        int task_size = pks[5].toInt();
        QString _job_name = pks[6].toString();
        int first_frame = pks[7].toInt();
        int last_frame = pks[8].toInt();

        options->priority_combobox->setCurrentIndex(priority);
        options->first_frame_edit->setText(QString::number(first_frame));
        options->last_frame_edit->setText(QString::number(last_frame));
        options->job_name_edit->setText(_job_name);
        options->task_size_edit->setText(QString::number(task_size));
        options->comment_edit->setText(comment);
        options->instances_edit->setText(QString::number(instances));

        // Agrega grupos al combobox y establese el item correspondiente
        options->group_combobox->clear();
        options->group_combobox->addItem("None");
        QString current_group = "None";
        for (int i = 0; i < groups->topLevelItemCount(); ++i)
        {
            QString name = groups->topLevelItem(i)->text(2);
            options->group_combobox->addItem(name);

            if (serverGroupExist.contains(name))
                current_group = name;
        }
        options->group_combobox->setCurrentText(current_group);
        // ---------------------------------
    }
}

void jobs_class::options_ok()
{
    QJsonArray machines = {};
    // guarda el grupo del combobox en el array
    QJsonArray group;
    QString current_group = options->group_combobox->currentText();
    group.push_back(current_group);
    // -------------------------------

    int priority = options->priority_combobox->currentIndex();
    int first_frame = options->first_frame_edit->text().toInt();
    int last_frame = options->last_frame_edit->text().toInt();
    int task_size = options->task_size_edit->text().toInt();
    QString comment = options->comment_edit->text();
    QString _job_name = options->job_name_edit->text();

    int instances = options->instances_edit->text().toInt();
    if (instances > 16)
        instances = 16;

    QJsonArray pks;
    auto selected = this->selectedItems();
    selected.push_front(first_job_item);

    QStringList repeatItem;
    for (auto item : selected)
    {
        QString job_name = item->text(0);
        QJsonArray options = {machines, group, priority, comment, instances, first_frame, last_frame, task_size, _job_name};
        // el primer item se repite asi que si esta 2 veces no lo agrega otra vez
        if (not repeatItem.contains(job_name))
            pks.push_back({{job_name, options, "write"}});
        repeatItem.push_back(job_name);
        //-------------------------------------------------------
    }
    pks = {"jobOptions", pks};

    QString ask = "Sure you want to send the changes? \nSome frames will be lost due to the size of the tasks.";

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(monitor, "Job Options", ask, QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        tcpClient(shared->manager_host, shared->manager_port, jats({3, pks}));
        properties->parentWidget()->hide();
    }
}

void jobs_class::message(
    void (jobs_class::*funtion)(QString),
    QString action,
    QString ask,
    QString tile,
    jobs_class *_class)
{
    auto selected = this->selectedItems();

    if (not selected.empty())
    {

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(monitor, tile, ask, QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            (_class->*funtion)(action);
        }
    }
}

void jobs_class::delete_start(QString action)
{

    auto root = this->invisibleRootItem();
    auto selected = this->selectedItems();

    QJsonArray pks;

    for (auto item : selected)
    {

        QString job_name = item->text(0);
        os::remove(path + "/log/trayIcon/" + job_name);

        pks.push_back({{job_name, action}});

        root->removeChild(item);
    }

    pks = {"jobAction", pks};

    tcpClient(shared->manager_host, shared->manager_port, jats({3, pks}));
}

void jobs_class::to_action(QString action)
{
    shared->stopUpdate = true;

    auto selected = this->selectedItems();

    QJsonArray pks;
    for (auto item : selected)
    {
        QString job_name = item->text(0);
        pks.push_back({{job_name, action}});
    }
    pks = {"jobAction", pks};

    tcpClient(shared->manager_host, shared->manager_port, jats({3, pks}));
}

void jobs_class::item_delete()
{
    shared->stopUpdate = true;

    QString ask = "Sure you want to delete the job?";
    QString tile = "Job Delete";
    QString action = "delete";
    message(&jobs_class::delete_start, action, ask, tile, this);
}

void jobs_class::mousePressEvent(QMouseEvent *event)
{
    QTreeView::mousePressEvent(event);

    bool selected = selectionModel()->isSelected(indexAt(event->pos()));
    // si se clickea no en un item, borra las tareas de la lista
    if (not selected)
        shared->tasks_tree->clear();
    //----------------------------------------------------
}
