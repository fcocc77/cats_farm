#include "../hpp/tasks.hpp"

tasks_class::tasks_class(
    QMainWindow *_monitor,
    shared_variables *_shared,
    jobs_class *_jobs)
{
    monitor = _monitor;
    shared = _shared;
    jobs = _jobs;

    // Task Action
    suspend_action = new QAction("Suspend");
    restart_action = new QAction("Restart");
    render_server_action = new QAction("Select server tasks");
    //------------------------------------------------

    setup_ui();
    connections();
}

tasks_class::~tasks_class()
{
}

void tasks_class::setup_ui()
{
    this->setObjectName("tasks");

    QStringList columns = {"Task", "Frame Range", "Status", "Server", "Task Time"};
    this->setColumnCount(5);
    this->setHeaderLabels(columns); // pone el nombre de las columnas

    this->setSelectionMode(QAbstractItemView::ExtendedSelection); // multi seleccion
    this->setAlternatingRowColors(true);                          // item con color alternativos
    this->setIndentation(0);
    this->setFocusPolicy(Qt::NoFocus); // elimina el margen del principio

    this->setColumnWidth(0, 100);
    this->setColumnWidth(1, 100);
    this->setColumnWidth(2, 100);
    this->setColumnWidth(3, 100);

    this->setSortingEnabled(true);
    this->sortByColumn(0, Qt::AscendingOrder);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
}

void tasks_class::connections()
{
    connect(this, &QTreeWidget::customContextMenuRequested, this, &tasks_class::popup);

    // Task Action
    connect(suspend_action, &QAction::triggered, this, [this]() { to_action("suspend"); });
    connect(restart_action, &QAction::triggered, this, &tasks_class::restart);

    connect(render_server_action, &QAction::triggered, this, &tasks_class::render_server);
    //-----------------------------------------------------
}

void tasks_class::popup()
{
    auto selected = this->selectedItems();
    if (not selected.empty())
    {
        QMenu *menu = new QMenu(monitor);

        menu->addAction(suspend_action);
        menu->addSeparator();
        menu->addAction(restart_action);
        menu->addAction(render_server_action);
        menu->popup(QCursor::pos());
    }
}

void tasks_class::restart()
{
    QString ask = "Are you Sure you want to restart the task?";
    QString tile = "Task Restart";
    QString action = "resume";

    message(action, ask, tile);
}

void tasks_class::render_server()
{
    auto selected = this->selectedItems();
    QString _server = selected[0]->text(3);
    _server = _server.split(":")[0];

    for (int i = 0; i < this->topLevelItemCount(); ++i)
    {
        auto item = this->topLevelItem(i);

        QString server = item->text(3);
        server = server.split(":")[0];
        if (_server == server)
            item->setSelected(true);
    }
}

void tasks_class::message(QString action, QString ask, QString tile)
{

    auto selected = this->selectedItems();
    if (not selected.empty())
    {

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(monitor, tile, ask, QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            to_action(action);
        }
    }
}

void tasks_class::to_action(QString action)
{

    QJsonArray pks;
    for (auto item_job : jobs->selectedItems())
    {
        QString job_name = item_job->text(0);

        for (auto item_task : this->selectedItems())
        {
            QString task_name = item_task->text(0);
            pks.push_back({{job_name, task_name, action}});
        }
    }

    pks = {"taskAction", pks};

    tcpClient(shared->manager_host, shared->manager_port, jats({3, pks}));
}
