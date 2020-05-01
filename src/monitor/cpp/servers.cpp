#include "../hpp/servers.hpp"

servers_class::servers_class(
    QMainWindow *_monitor,
    shared_variables *_shared,
    log_class *_log)
{
    monitor = _monitor;
    shared = _shared;
    log = _log;

    // display job list
    show_all_action = new QAction("Show All");
    hide_all_action = new QAction("Hide All");
    display_windows_action = new QAction("Windows");
    display_linux_action = new QAction("Linux");
    display_mac_action = new QAction("Mac OS X");
    display_on_action = new QAction("ON");
    display_off_action = new QAction("OFF");
    //------------------------------------------------

    // Server Action
    server_inactive_action = new QAction("Disable");
    server_reactive_action = new QAction("Enable");
    server_max_instances_action = new QAction("Max Instances");
    server_free_ram_action = new QAction("Free Ram");
    server_turn_on_action = new QAction("Turn ON");
    server_turn_off_action = new QAction("Turn OFF");
    server_ssh_action = new QAction("SSH");
    server_show_log = new QAction("Show Log");
    server_vnc_action = new QAction("VNC");
    delete_action = new QAction("Delete Server");
    //------------------------------------------------

    connections();
    setup_ui();
}

servers_class::~servers_class()
{
}

void servers_class::setup_ui()
{
    this->setObjectName("servers");

    QStringList columns{"Server Name", "Status", "Ins.", "CPU Usage", "CPU Temp",
                        "RAM Usage", "System", "IP", "MAC Address", "Job Rendered"};
    this->setHeaderLabels(columns);

    this->setSelectionMode(QAbstractItemView::ExtendedSelection); // multi seleccion
    this->setIndentation(0);                                      // elimina el margen del principio
    this->setAlternatingRowColors(true);
    this->setFocusPolicy(Qt::NoFocus); // item con color alternativos

    this->setGeometry(1000, 1000, 1000, 1000);

    // ajusta el largo de las columnas
    this->setColumnWidth(0, 150);
    this->setColumnWidth(1, 50);
    this->setColumnWidth(2, 50);
    this->setColumnWidth(3, 177);
    this->setColumnWidth(4, 100);
    this->setColumnWidth(5, 177);
    this->setColumnWidth(6, 100);
    //-------------------------------

    this->setContextMenuPolicy(Qt::CustomContextMenu);

    this->setSortingEnabled(true);
    this->sortByColumn(0, Qt::AscendingOrder);

    // Menus de display de servidores
    return;

    QMenuBar *menuBar = new QMenuBar(this);
    menuBar->addMenu("File");
    menuBar->addMenu("Edit");

    QMenu *display = new QMenu("Filter", monitor);
    menuBar->addMenu(display);

    display->addAction(show_all_action);
    display->addAction(hide_all_action);
    display->addSeparator();

    display->addAction(display_on_action);
    display->addAction(display_off_action);
    display->addSeparator();

    display->addAction(display_windows_action);
    display->addAction(display_linux_action);
    display->addAction(display_mac_action);
    //------------------------------------------
}

void servers_class::connections()
{
    shared->server_display = jread(path + "/etc/server_display.json");

    auto displayAction = [this](QString action) {
        if (shared->server_display[action].toBool())
            shared->server_display[action] = false;
        else
            shared->server_display[action] = true;
        jwrite(path + "/etc/server_display.json", shared->server_display);
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

        jwrite(path + "/etc/server_display.json", shared->server_display);
    };

    connect(show_all_action, &QAction::triggered, this, [=]() { displayAll(true); });
    connect(hide_all_action, &QAction::triggered, this, [=]() { displayAll(false); });

    connect(display_windows_action, &QAction::triggered, this, [=]() { displayAction("window"); });
    display_windows_action->setCheckable(true);
    display_windows_action->setChecked(shared->server_display["window"].toBool());

    connect(display_linux_action, &QAction::triggered, this, [=]() { displayAction("linux"); });
    display_linux_action->setCheckable(true);
    display_linux_action->setChecked(shared->server_display["linux"].toBool());

    connect(display_mac_action, &QAction::triggered, this, [=]() { displayAction("mac"); });
    display_mac_action->setCheckable(true);
    display_mac_action->setChecked(shared->server_display["mac"].toBool());

    connect(display_on_action, &QAction::triggered, this, [=]() { displayAction("on"); });
    display_on_action->setCheckable(true);
    display_on_action->setChecked(shared->server_display["on"].toBool());

    connect(display_off_action, &QAction::triggered, this, [=]() { displayAction("off"); });
    display_off_action->setCheckable(true);
    display_off_action->setChecked(shared->server_display["off"].toBool());

    // Server List Connections

    connect(this, &QTreeWidget::customContextMenuRequested, this, &servers_class::server_popup);

    // Server Action

    connect(delete_action, &QAction::triggered, this, [this]() {
        QString ask = "Are you sure to want delete this server?";
        QString tile = "Server Delete";
        QString action = "delete";

        message(&servers_class::to_action, action, ask, tile, "None", this);
    });

    connect(server_inactive_action, &QAction::triggered, this, [this]() {
        QString ask = "The task will kill, Sure you want to disable this server?";
        QString tile = "Server Disabling";
        QString action = "inactive";
        message(&servers_class::to_action, action, ask, tile, "None", this);
    });
    connect(server_reactive_action, &QAction::triggered, this, [this]() {
        to_action("reactive", "None");
    });
    connect(server_max_instances_action, &QAction::triggered, this, &servers_class::server_max_instances);
    connect(server_free_ram_action, &QAction::triggered, this, [this]() {
        send_to_vserver("freeram", "none");
    });

    connect(server_turn_on_action, &QAction::triggered, this, [this]() {
        to_action("turn_on", "none");
    });
    connect(server_turn_off_action, &QAction::triggered, this, [this]() {
        QString ask = "Are you sure to want turn off this server?";
        QString tile = "Server OFF";
        QString action = "off";

        message(&servers_class::send_to_vserver, action, ask, tile, "None", this);
    });

    connect(server_ssh_action, &QAction::triggered, this, &servers_class::ssh_client);
    connect(server_show_log, &QAction::triggered, this, &servers_class::to_log);
    server_show_log->setShortcut(QString("Ctrl+L"));
    connect(server_vnc_action, &QAction::triggered, this, &servers_class::vnc_client);
    //--------------------------------------------------------
}

void servers_class::server_popup()
{
    auto selected = this->selectedItems();

    QMenu *menu = new QMenu(monitor);
    if (not selected.empty())
    {

        menu->addAction(server_inactive_action);
        menu->addAction(server_reactive_action);
        menu->addAction(delete_action);

        menu->addSeparator();
        menu->addAction(server_show_log);
        menu->addSeparator();

        //-------------------------------------------------
        QMenu *submenu = new QMenu("Number of instances", monitor);
        QSignalMapper *mapper = new QSignalMapper(monitor);

        for (int i = 1; i < 16; ++i)
        {
            QString n = QString::number(i);
            QAction *action = new QAction(n + " Instances");
            connect(action, SIGNAL(triggered()), mapper, SLOT(map()));
            mapper->setMapping(action, i);
            submenu->addAction(action);
        }
        connect(mapper, SIGNAL(mapped(int)), this, SLOT(server_max_instances(int)));

        menu->addMenu(submenu);
        //-------------------------------------------------

        menu->addAction(server_free_ram_action);
        menu->addAction(server_turn_on_action);
        menu->addAction(server_turn_off_action);
        menu->addSeparator();
        menu->addAction(server_ssh_action);
        menu->addAction(server_vnc_action);
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

    auto selected = this->selectedItems();
    if (not selected.empty())
    {
        QString host = (selected[0]->text(7));

        QJsonArray send = {host, QJsonArray({1, true})};
        QString result = tcpClient(shared->manager_host, shared->manager_port, jats({5, send}));

        log->code_editor->setPlainText(result);
        log->parentWidget()->show();
    }
}

void servers_class::server_max_instances(int ins)
{
    to_action("max_instances", QString::number(ins));
}

void servers_class::ssh_client()
{

    QJsonArray recv = jafs(to_action("ssh", "None"));

    QString sshUser = recv[0].toString();
    QString sshPass = recv[1].toString();
    QString ip = recv[2].toString();

    QString disable_ask = " -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no ";
    QString cmd;
    QString ssh = "C:/vinarender/os/win/OpenSSH/bin/ssh.exe";
    QString linux_sshpass = "sshpass -p " + sshPass + " ssh -o StrictHostKeyChecking=no " + sshUser + "@" + ip;

    if (_win32)
        cmd = "cmd.exe /K start cmd.exe /K " + ssh + " -tt " + disable_ask + sshUser + "@" + shared->manager_host + " \"" + linux_sshpass + "\"";
    else
        cmd = "gnome-terminal -e '" + linux_sshpass + "'";
    os::back(cmd);
}

void servers_class::vnc_client()
{

    QString tigervnc;
    if (_linux)
    {
        tigervnc = "vncviewer";
    }
    else if (_win32)
    {
        tigervnc = path + "/os/win/vnc/vncviewer.exe";
    }
    else
    {
        tigervnc = path + "/os/mac/vnc/vncviewer";
    }

    for (auto item : this->selectedItems())
    {
        QString ip = item->text(7);
        QString arg = "\"" + tigervnc + "\" " + ip + ":7";
        os::back(arg);
    }
}

void servers_class::message(
    QString (servers_class::*funtion)(QString, QString),
    QString action,
    QString ask,
    QString tile,
    QString info,
    servers_class *_class)
{
    auto selected = this->selectedItems();
    if (not selected.empty())
    {

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(monitor, tile, ask, QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            (_class->*funtion)(action, info);
        }
    }
}

QString servers_class::to_action(QString action, QString info)
{
    QJsonArray pks;
    auto selected = this->selectedItems();
    for (auto item : selected)
    {
        QString server_name = item->text(0);
        if (not item->parent())
        {
            pks.push_back({{server_name, action, info}});
        }
    }
    pks = {"serverAction", pks};

    QString recv = tcpClient(shared->manager_host, shared->manager_port, jats({3, pks}));

    return recv;
}

QString servers_class::send_to_vserver(QString action, QString info)
{
    QList<QTreeWidgetItem *> selected = this->selectedItems();

    for (auto item : selected)
    {
        QString host = item->text(7);

        QJsonArray send = {host, QJsonArray({4, {{action, info}}})};
        tcpClient(shared->manager_host, shared->manager_port, jats({5, send}));
    }
    //-------------------------------------
    return "none";
}
