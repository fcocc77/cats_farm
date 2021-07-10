#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QPixmap>

#include "../global/global.h"
#include "jobs_widget.h"
#include "main_window.h"
#include "tcp.h"
#include "util.h"

jobs_class::jobs_class(shared_variables *_shared, QWidget *__monitor,
                       log_class *_log, servers_class *_servers,
                       options_class *_options, groups_class *_groups,
                       properties_class *_properties, submit *__submit)

    : shared(_shared)
    , _monitor(__monitor)
    , log_widget(_log)
    , servers(_servers)
    , options(_options)
    , groups(_groups)
    , properties(_properties)
    , _submit(__submit)
    , tree(new QTreeWidget)
    , layout(new QVBoxLayout(this))
    , _title_bar(new title_bar("Jobs"))
{
    this->setAcceptDrops(true);

    delete_action = new QAction("Delete");
    job_pause_action = new QAction("Pause");
    job_resume_action = new QAction("Resume");
    job_restart_action = new QAction("Restart");
    job_unlock_servers_action = new QAction("Enable blocked servers");
    job_log_action = new QAction("Show Log");
    job_modify_action = new QAction("Modify");

    auto icon = [=](QString name) {
        return QIcon(VINARENDER_PATH + "/resources/images/" + name +
                     "_normal.png");
    };

    delete_action->setIcon(icon("delete"));
    job_pause_action->setIcon(icon("pause"));
    job_resume_action->setIcon(icon("play_arrow"));
    job_restart_action->setIcon(icon("restart"));
    job_unlock_servers_action->setIcon(icon("unlock"));
    job_log_action->setIcon(icon("log"));
    job_modify_action->setIcon(icon("edit"));

    setup_ui();
    connections();
}

jobs_class::~jobs_class() {}

void jobs_class::setup_ui()
{
    this->setObjectName("jobs");

    layout->setMargin(0);
    layout->setSpacing(0);

    tree->setColumnCount(10);
    QStringList columns{"Job Name",
                        "Priority",
                        "Software",
                        "Task Progress",
                        "Status",
                        "Submit Data/Time",
                        "Finished Data/Time",
                        "Estimated Time",
                        "Total Render Time",
                        "Errors"};

    tree->setHeaderLabels(columns);

    tree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tree->setAlternatingRowColors(true);
    tree->setIndentation(0);

    tree->setColumnWidth(0, 200);
    tree->setColumnWidth(1, 80);
    tree->setColumnWidth(2, 100);
    tree->setColumnWidth(3, 200);
    tree->setColumnWidth(4, 100);
    tree->setColumnWidth(5, 150);
    tree->setColumnWidth(6, 150);
    tree->setColumnWidth(7, 150);
    tree->setColumnWidth(8, 150);

    tree->setContextMenuPolicy(Qt::CustomContextMenu);

    tree->setSortingEnabled(true);
    tree->setFocusPolicy(Qt::NoFocus);
    tree->sortByColumn(5, Qt::AscendingOrder);

    // Layout
    layout->addWidget(_title_bar);
    layout->addWidget(tree);
}

void jobs_class::connections()
{
    connect(tree, &QTreeWidget::itemDoubleClicked, options,
            &options_class::open_panel);

    connect(tree, &QTreeWidget::itemSelectionChanged, this,
            &jobs_class::selection_changed);

    connect(tree, &QTreeWidget::customContextMenuRequested, this,
            &jobs_class::popup);

    connect(delete_action, &QAction::triggered, this, &jobs_class::item_delete);

    delete_action->setShortcut(QString("Del"));

    connect(job_pause_action, &QAction::triggered, this, [this]() {
        QString ask = "Sure you want to pause the job?";
        QString tile = "Job Pause";
        QString action = "pause";
        message(&jobs_class::to_action, action, ask, tile, this);
    });

    connect(job_restart_action, &QAction::triggered, this, [this]() {
        QString ask = "Sure you want to restart the job?";
        QString tile = "Job Restart";
        QString action = "restart";
        message(&jobs_class::to_action, action, ask, tile, this);
    });

    connect(job_resume_action, &QAction::triggered, this,
            [this]() { to_action("resume"); });

    connect(job_unlock_servers_action, &QAction::triggered, this,
            [this]() { to_action("unlock"); });

    connect(job_log_action, &QAction::triggered, this, &jobs_class::show_log);

    job_log_action->setShortcut(QString("L"));
}

void jobs_class::popup()
{
    auto selected = tree->selectedItems();
    if (not selected.empty())
    {
        QMenu *menu = new QMenu(_monitor);

        menu->addAction(job_pause_action);
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
    auto selected = tree->selectedItems();
    if (selected.isEmpty())
        return;

    QString job_name = selected[0]->text(0);

    // encuentra job seleccionado en los resividos del manager
    QJsonObject job;
    for (QJsonValue j : shared->jobs)
    {
        QJsonObject _job = j.toObject();
        if (_job["name"].toString() == job_name)
            job = _job;
    }

    QStringList servers = {};
    for (QJsonValue _task : job["task"].toArray())
    {
        QJsonObject task = _task.toObject();
        QString server = task["server"].toString().split(':')[0];

        if (server != "..." && !servers.contains(server))
            servers.push_back(server);
    }

    if (!log_widget->isVisible())
        properties->switch_widget("log");

    if (servers.isEmpty())
    {
        log_widget->set_text("The jobs has not yet rendered");
        return;
    }

    QString software = job["software"].toString().toLower();
    QString log_servers;

    for (QString server : servers)
    {
        QString server_ip = shared->servers[server].toObject()["ip"].toString();

        QJsonArray send = {server_ip, QJsonArray({1, ""})};
        QString recv =
            tcpClient(shared->manager_host, MANAGER_PORT, jats({5, send}));

        log_servers += jofs(recv)[software].toString();
    }

    log_widget->set_text(log_servers);
}

void jobs_class::message(void (jobs_class::*funtion)(QString), QString action,
                         QString ask, QString tile, jobs_class *_class)
{
    auto selected = tree->selectedItems();

    if (not selected.empty())
    {

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(_monitor, tile, ask,
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            (_class->*funtion)(action);
        }
    }
}

void jobs_class::delete_start(QString action)
{

    auto root = tree->invisibleRootItem();
    auto selected = tree->selectedItems();

    QJsonArray pks;

    for (auto item : selected)
    {

        QString job_name = item->text(0);
        os::remove(VINARENDER_PATH + "/log/trayIcon/" + job_name);

        pks.push_back({{job_name, action}});

        root->removeChild(item);
    }

    pks = {"jobAction", pks};

    tcpClient(shared->manager_host, MANAGER_PORT, jats({3, pks}));
}

void jobs_class::to_action(QString action)
{
    shared->stopUpdate = true;

    auto selected = tree->selectedItems();

    QJsonArray pks;
    for (auto item : selected)
    {
        QString job_name = item->text(0);
        pks.push_back({{job_name, action}});
    }
    pks = {"jobAction", pks};

    tcpClient(shared->manager_host, MANAGER_PORT, jats({3, pks}));
}

void jobs_class::item_delete()
{
    shared->stopUpdate = true;

    QString ask = "Sure you want to delete the job?";
    QString tile = "Job Delete";
    QString action = "delete";
    message(&jobs_class::delete_start, action, ask, tile, this);
}

void jobs_class::selection_changed()
{
    if (shared->tasks_tree->isVisible())
        static_cast<monitor *>(_monitor)->get_update()->get_task();

    options->update_panel();
}

void jobs_class::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void jobs_class::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void jobs_class::dropEvent(QDropEvent *event)
{
    QStringList files;

    for (QUrl url : event->mimeData()->urls())
    {
        QString file = url.toLocalFile();

        if (os::isdir(file))
        {
            for (QString deep_file : os::listdir(file))
                if (!os::isdir(deep_file))
                    files.push_back(deep_file);
        }
        else
            files.push_back(file);
    }

    _submit->submit_files(files);

    event->acceptProposedAction();
}
