#include <QAction>
#include <QMenuBar>
#include <QSignalMapper>

#include "../global/global.h"
#include "servers_widget.h"
#include "tcp.h"
#include "util.h"

servers_class::servers_class(QWidget *__monitor, shared_variables *_shared,
                             log_class *_log)

    : _monitor(__monitor)
    , shared(_shared)
    , log(_log)
    , layout(new QVBoxLayout(this))
    , tree(new QTreeWidget)
{
    // display job list
    show_all_action = new QAction("Show All");
    hide_all_action = new QAction("Hide All");
    display_windows_action = new QAction("Windows");
    display_linux_action = new QAction("Linux");
    display_mac_action = new QAction("Mac OS X");
    display_on_action = new QAction("ON");
    display_off_action = new QAction("OFF");

    // Server Action
    server_inactive_action = new QAction("Disable");
    server_reactive_action = new QAction("Enable");
    server_max_instances_action = new QAction("Max Instances");
    server_free_ram_action = new QAction("Free Ram");
    server_turn_on_action = new QAction("Turn ON");
    server_turn_off_action = new QAction("Turn OFF");
    delete_action = new QAction("Delete Server");


    auto icon = [=](QString name) {
        return QIcon(VINARENDER_PATH + "/resources/images/" + name +
                     "_normal.png");
    };

    server_inactive_action->setIcon(icon("disable_server"));
    server_reactive_action->setIcon(icon("enable_server"));
    delete_action->setIcon(icon("delete"));
    server_turn_on_action->setIcon(icon("on"));
    server_turn_off_action->setIcon(icon("off"));
    server_free_ram_action->setIcon(icon("ram"));

    connections();
    setup_ui();
}

servers_class::~servers_class() {}

void servers_class::setup_ui()
{
    layout->setMargin(0);
    layout->setSpacing(0);

    this->setObjectName("servers");

    QStringList columns{"Host Name",   "Status",      "Ins.",   "CPU Usage",
                        "CPU Temp",    "RAM Usage",   "System", "IP",
                        "MAC Address", "Job Rendered"};

    tree->setHeaderLabels(columns);
    tree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tree->setIndentation(0);
    tree->setAlternatingRowColors(true);
    tree->setFocusPolicy(Qt::NoFocus);

    tree->setGeometry(1000, 1000, 1000, 1000);

    // ajusta el largo de las columnas
    tree->setColumnWidth(0, 150);
    tree->setColumnWidth(1, 100);
    tree->setColumnWidth(2, 50);
    tree->setColumnWidth(3, 177);
    tree->setColumnWidth(4, 100);
    tree->setColumnWidth(5, 177);
    tree->setColumnWidth(6, 100);
    tree->setColumnWidth(8, 130);

    tree->setContextMenuPolicy(Qt::CustomContextMenu);

    tree->setSortingEnabled(true);
    tree->sortByColumn(0, Qt::AscendingOrder);

    _title_bar = new title_bar("Servers");

    // Layout
    layout->addWidget(_title_bar);
    layout->addWidget(tree);
}

void servers_class::connections()
{
    shared->server_display =
        jread(VINARENDER_CONF_PATH + "/server_display.json");

    auto displayAction = [this](QString action) {
        if (shared->server_display[action].toBool())
            shared->server_display[action] = false;
        else
            shared->server_display[action] = true;
        jwrite(VINARENDER_CONF_PATH + "/server_display.json",
               shared->server_display);
    };

    auto displayAll = [this](bool status) {
        shared->server_display["mac"] = status;
        shared->server_display["linux"] = status;
        shared->server_display["window"] = status;
        shared->server_display["on"] = status;
        shared->server_display["off"] = status;
        shared->server_display["show"] = status;
        shared->server_display["hide"] = status;

        display_windows_action->setChecked(status);
        display_linux_action->setChecked(status);
        display_mac_action->setChecked(status);
        display_on_action->setChecked(status);
        display_off_action->setChecked(status);

        jwrite(VINARENDER_CONF_PATH + "/server_display.json",
               shared->server_display);
    };

    connect(show_all_action, &QAction::triggered, this,
            [=]() { displayAll(true); });
    connect(hide_all_action, &QAction::triggered, this,
            [=]() { displayAll(false); });

    connect(display_windows_action, &QAction::triggered, this,
            [=]() { displayAction("window"); });
    display_windows_action->setCheckable(true);
    display_windows_action->setChecked(
        shared->server_display["window"].toBool());

    connect(display_linux_action, &QAction::triggered, this,
            [=]() { displayAction("linux"); });
    display_linux_action->setCheckable(true);
    display_linux_action->setChecked(shared->server_display["linux"].toBool());

    connect(display_mac_action, &QAction::triggered, this,
            [=]() { displayAction("mac"); });
    display_mac_action->setCheckable(true);
    display_mac_action->setChecked(shared->server_display["mac"].toBool());

    connect(display_on_action, &QAction::triggered, this,
            [=]() { displayAction("on"); });
    display_on_action->setCheckable(true);
    display_on_action->setChecked(shared->server_display["on"].toBool());

    connect(display_off_action, &QAction::triggered, this,
            [=]() { displayAction("off"); });
    display_off_action->setCheckable(true);
    display_off_action->setChecked(shared->server_display["off"].toBool());

    // Server List Connections

    connect(tree, &QTreeWidget::customContextMenuRequested, this,
            &servers_class::server_popup);

    // Server Action

    connect(delete_action, &QAction::triggered, this, [this]() {
        QString ask = "Are you sure to want delete this server?";
        QString tile = "Server Delete";
        QString action = "delete";

        message(&servers_class::to_action, action, ask, tile, "None", this);
    });

    connect(server_inactive_action, &QAction::triggered, this, [this]() {
        QString ask =
            "The task will kill, Sure you want to disable this server?";
        QString tile = "Server Disabling";
        QString action = "inactive";
        message(&servers_class::to_action, action, ask, tile, "None", this);
    });
    connect(server_reactive_action, &QAction::triggered, this,
            [this]() { to_action("reactive", "None"); });
    connect(server_max_instances_action, &QAction::triggered, this,
            &servers_class::server_max_instances);
    connect(server_free_ram_action, &QAction::triggered, this,
            [this]() { send_to_vserver("freeram", "none"); });

    connect(server_turn_on_action, &QAction::triggered, this,
            [this]() { to_action("turn_on", "none"); });
    connect(server_turn_off_action, &QAction::triggered, this, [this]() {
        QString ask = "Are you sure to want turn off this server?";
        QString tile = "Server OFF";
        QString action = "off";

        message(&servers_class::send_to_vserver, action, ask, tile, "None",
                this);
    });
}

void servers_class::server_popup()
{
    auto selected = tree->selectedItems();

    QMenu *menu = new QMenu(_monitor);
    if (not selected.empty())
    {

        menu->addAction(server_inactive_action);
        menu->addAction(server_reactive_action);
        menu->addAction(delete_action);

        menu->addSeparator();

        QMenu *submenu = new QMenu("Number of instances", _monitor);
        QSignalMapper *mapper = new QSignalMapper(_monitor);

        for (int i = 1; i < 16; ++i)
        {
            QString n = QString::number(i);
            QAction *action = new QAction(n + " Instances");
            connect(action, SIGNAL(triggered()), mapper, SLOT(map()));
            mapper->setMapping(action, i);
            submenu->addAction(action);
        }
        connect(mapper, SIGNAL(mapped(int)), this,
                SLOT(server_max_instances(int)));

        menu->addMenu(submenu);

        menu->addAction(server_free_ram_action);
        menu->addAction(server_turn_on_action);
        menu->addAction(server_turn_off_action);
        menu->addSeparator();
        menu->popup(QCursor::pos());
    }
    else
    {

        menu->addAction(show_all_action);
        menu->addAction(hide_all_action);
        menu->addSeparator();
        menu->addAction(display_on_action);
        menu->addAction(display_off_action);
        menu->addSeparator();
        menu->addAction(display_windows_action);
        menu->addAction(display_linux_action);
        menu->addAction(display_mac_action);
        menu->popup(QCursor::pos());
    }
}

void servers_class::cpu_limit(int limit)
{
    send_to_vserver("cpu_limit", QString::number(limit));
}

void servers_class::to_log()
{

    auto selected = tree->selectedItems();
    if (not selected.empty())
    {
        QString host = (selected[0]->text(7));

        QJsonArray send = {host, QJsonArray({1, true})};
        QString result =
            tcpClient(shared->manager_host, MANAGER_PORT, jats({5, send}));

        log->set_text(result);
        log->parentWidget()->show();
    }
}

void servers_class::server_max_instances(int ins)
{
    to_action("max_instances", QString::number(ins));
}

void servers_class::message(QString (servers_class::*funtion)(QString, QString),
                            QString action, QString ask, QString tile,
                            QString info, servers_class *_class)
{
    auto selected = tree->selectedItems();
    if (not selected.empty())
    {

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(_monitor, tile, ask,
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            (_class->*funtion)(action, info);
        }
    }
}

QString servers_class::to_action(QString action, QString info)
{
    QJsonArray pks;
    auto selected = tree->selectedItems();
    for (auto item : selected)
    {
        QString server_name = item->text(0);
        if (not item->parent())
        {
            pks.push_back({{server_name, action, info}});
        }
    }
    pks = {"serverAction", pks};

    QString recv =
        tcpClient(shared->manager_host, MANAGER_PORT, jats({3, pks}));

    return recv;
}

QString servers_class::send_to_vserver(QString action, QString info)
{
    QList<QTreeWidgetItem *> selected = tree->selectedItems();

    for (auto item : selected)
    {
        QString host = item->text(7);

        QJsonArray send = {host, QJsonArray({4, {{action, info}}})};
        tcpClient(shared->manager_host, MANAGER_PORT, jats({5, send}));
    }

    return "none";
}
