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
    , log(_log)
    , servers(_servers)
    , options(_options)
    , groups(_groups)
    , properties(_properties)
    , _submit(__submit)
{
    this->setAcceptDrops(true);

    delete_action = new QAction("Delete");
    job_suspend_action = new QAction("Suspend");
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
    job_suspend_action->setIcon(icon("pause"));
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
    this->setColumnCount(10);
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

    this->setHeaderLabels(columns);

    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->setAlternatingRowColors(true);
    this->setIndentation(0);

    this->setColumnWidth(0, 200);
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
    connect(this, &QTreeWidget::itemDoubleClicked, options,
            &options_class::open_panel);

    connect(this, &QTreeWidget::customContextMenuRequested, this,
            &jobs_class::popup);

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

    connect(job_resume_action, &QAction::triggered, this,
            [this]() { to_action("resume"); });

    connect(job_unlock_servers_action, &QAction::triggered, this,
            [this]() { to_action("unlock"); });

    connect(job_log_action, &QAction::triggered, this, &jobs_class::show_log);

    job_log_action->setShortcut(QString("L"));
}

void jobs_class::popup()
{
    auto selected = this->selectedItems();
    if (not selected.empty())
    {
        QMenu *menu = new QMenu(_monitor);

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
                QString result =
                    tcpClient(shared->manager_host, shared->manager_port,
                              jats({5, send}));

                log->set_text(_name + " Log:\n\n" + result);
                break;
            }
            else
                log->set_text("The jobs has not yet rendered");
        }

        properties->switch_widget("log");
    }
}

void jobs_class::message(void (jobs_class::*funtion)(QString), QString action,
                         QString ask, QString tile, jobs_class *_class)
{
    auto selected = this->selectedItems();

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

    auto root = this->invisibleRootItem();
    auto selected = this->selectedItems();

    QJsonArray pks;

    for (auto item : selected)
    {

        QString job_name = item->text(0);
        os::remove(VINARENDER_PATH + "/log/trayIcon/" + job_name);

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

    bool selected_item = selectionModel()->isSelected(indexAt(event->pos()));

    if (shared->tasks_tree->isVisible())
    {
        static_cast<monitor *>(_monitor)->get_update()->get_task();
        if (!selected_item)
            shared->tasks_tree->clear();
    }

    options->update_panel(!selected_item);
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
