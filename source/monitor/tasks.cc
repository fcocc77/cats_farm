#include <tasks.h>

tasks_class::tasks_class(
    QMainWindow *_monitor,
    shared_variables *_shared,
    jobs_class *_jobs)

    : monitor(_monitor),
      shared(_shared),
      jobs(_jobs)
{
    // Task Action
    suspend_action = new QAction("Suspend");
    restart_action = new QAction("Restart");
    render_server_action = new QAction("Select server tasks");
    //------------------------------------------------

    tree = new QTreeWidget();
    shared->tasks_tree = tree;

    setup_ui();
    connections();
}

tasks_class::~tasks_class()
{
}

void tasks_class::setup_ui()
{
    tree->setObjectName("tasks");

    QStringList columns = {"Task", "Frame Range", "Status", "Server", "Task Time"};
    tree->setColumnCount(5);
    tree->setHeaderLabels(columns); // pone el nombre de las columnas

    tree->setSelectionMode(QAbstractItemView::ExtendedSelection); // multi seleccion
    tree->setAlternatingRowColors(true);                          // item con color alternativos
    tree->setIndentation(0);
    tree->setFocusPolicy(Qt::NoFocus); // elimina el margen del principio

    tree->setColumnWidth(0, 100);
    tree->setColumnWidth(1, 100);
    tree->setColumnWidth(2, 100);
    tree->setColumnWidth(3, 100);

    tree->setSortingEnabled(true);
    tree->sortByColumn(0, Qt::AscendingOrder);

    tree->setContextMenuPolicy(Qt::CustomContextMenu);
}

void tasks_class::connections()
{
    connect(tree, &QTreeWidget::customContextMenuRequested, this, &tasks_class::popup);

    // Task Action
    connect(suspend_action, &QAction::triggered, this, [this]() { to_action("suspend"); });
    connect(restart_action, &QAction::triggered, this, &tasks_class::restart);

    connect(render_server_action, &QAction::triggered, this, &tasks_class::render_server);
    //-----------------------------------------------------
}

void tasks_class::popup()
{
    auto selected = tree->selectedItems();
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
    auto selected = tree->selectedItems();
    QString _server = selected[0]->text(3);
    _server = _server.split(":")[0];

    for (int i = 0; i < tree->topLevelItemCount(); ++i)
    {
        auto item = tree->topLevelItem(i);

        QString server = item->text(3);
        server = server.split(":")[0];
        if (_server == server)
            item->setSelected(true);
    }
}

void tasks_class::message(QString action, QString ask, QString tile)
{

    auto selected = tree->selectedItems();
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

        for (auto item_task : tree->selectedItems())
        {
            QString task_name = item_task->text(0);
            pks.push_back({{job_name, task_name, action}});
        }
    }

    pks = {"taskAction", pks};

    tcpClient(shared->manager_host, shared->manager_port, jats({3, pks}));
}
