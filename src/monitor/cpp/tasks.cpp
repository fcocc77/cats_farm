#include "../hpp/tasks.hpp"

tasks_class::tasks_class(
    Ui::MainWindow *_ui,
    QMainWindow *_monitor)
{
    ui = _ui;
    monitor = _monitor;
    manager_host = fread(path + "/etc/manager_host");

    // Task Action
    suspend_action = new QAction("Suspend");
    restart_action = new QAction("Restart");
    render_server_action = new QAction("Select server tasks");
    //------------------------------------------------

    properties();
    connections();
}

tasks_class::~tasks_class()
{
}

void tasks_class::properties()
{
    ui->tasks->setSelectionMode(QAbstractItemView::ExtendedSelection); // multi seleccion
    ui->tasks->setAlternatingRowColors(true);                          // item con color alternativos
    ui->tasks->setIndentation(0);                                      // elimina el margen del principio

    ui->tasks->setColumnWidth(0, 100);
    ui->tasks->setColumnWidth(1, 100);
    ui->tasks->setColumnWidth(2, 100);
    ui->tasks->setColumnWidth(3, 100);

    ui->tasks->setSortingEnabled(true);
    ui->tasks->sortByColumn(0, Qt::AscendingOrder);

    ui->tasks->setContextMenuPolicy(Qt::CustomContextMenu);
}

void tasks_class::connections()
{

    connect(ui->tasks, &QTreeWidget::customContextMenuRequested, this, &tasks_class::popup);

    // Task Action
    connect(suspend_action, &QAction::triggered, this, [this]() { to_action("suspend"); });
    connect(restart_action, &QAction::triggered, this, &tasks_class::restart);

    connect(render_server_action, &QAction::triggered, this, &tasks_class::render_server);

    //-----------------------------------------------------
}

void tasks_class::popup()
{
    auto selected = ui->tasks->selectedItems();
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
    auto selected = ui->tasks->selectedItems();
    QString _server = selected[0]->text(3);
    _server = _server.split(":")[0];

    for (int i = 0; i < ui->tasks->topLevelItemCount(); ++i)
    {
        auto item = ui->tasks->topLevelItem(i);

        QString server = item->text(3);
        server = server.split(":")[0];
        if (_server == server)
            item->setSelected(true);
    }
}

void tasks_class::message(QString action, QString ask, QString tile)
{

    auto selected = ui->tasks->selectedItems();
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
    for (auto item_job : ui->jobs->selectedItems())
    {
        QString job_name = item_job->text(0);

        for (auto item_task : ui->tasks->selectedItems())
        {
            QString task_name = item_task->text(0);
            pks.push_back({{job_name, task_name, action}});
        }
    }

    pks = {"taskAction", pks};

    tcpClient(manager_host, 7000, jats({3, pks}));
}
