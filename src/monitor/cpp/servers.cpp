#include "../hpp/servers.hpp"

servers_class::servers_class(
    Ui::MainWindow *_ui,
    QMainWindow *_monitor,
    shared_variables *_shared,
    log_class *_log)
{
    ui = _ui;
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
    server_ssh_action = new QAction("SSH");
    server_show_log = new QAction("Show Log");
    server_vnc_action = new QAction("VNC");
    delete_action = new QAction("Delete Server");
    //------------------------------------------------

    properties();
    connections();
}

servers_class::~servers_class()
{
}

void servers_class::properties()
{

    ui->servers->setSelectionMode(QAbstractItemView::ExtendedSelection); // multi seleccion
    ui->servers->setIndentation(0);                                      // elimina el margen del principio
    ui->servers->setAlternatingRowColors(true);                          // item con color alternativos

    ui->servers->setGeometry(1000, 1000, 1000, 1000);

    // ajusta el largo de las columnas
    ui->servers->setColumnWidth(0, 150);
    ui->servers->setColumnWidth(1, 50);
    ui->servers->setColumnWidth(2, 50);
    ui->servers->setColumnWidth(3, 177);
    ui->servers->setColumnWidth(4, 100);
    ui->servers->setColumnWidth(5, 177);
    ui->servers->setColumnWidth(6, 100);
    //-------------------------------

    ui->servers->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->servers->setSortingEnabled(true);
    ui->servers->sortByColumn(0, Qt::AscendingOrder);

    // Menus de display de servidores
    QMenuBar *menuBar = new QMenuBar();
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

    // QWidget *widget = new QWidget();
    // QVBoxLayout *vbox = new QVBoxLayout();
    // vbox->addWidget(menuBar);
    // vbox->addWidget(ui->servers);

    // widget->setLayout(vbox);

    // dock->setObjectName("Server");
    // dock->setWidget(widget);
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

    connect(ui->servers, &QTreeWidget::customContextMenuRequested, this, &servers_class::server_popup);

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
    connect(server_ssh_action, &QAction::triggered, this, &servers_class::ssh_client);
    connect(server_show_log, &QAction::triggered, this, &servers_class::to_log);
    server_show_log->setShortcut(QString("Ctrl+L"));
    connect(server_vnc_action, &QAction::triggered, this, &servers_class::vnc_client);
    //--------------------------------------------------------
}

void servers_class::server_popup()
{
    auto selected = ui->servers->selectedItems();

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

    auto selected = ui->servers->selectedItems();
    if (not selected.empty())
    {
        QString host = (selected[0]->text(7));

        QJsonArray send = {host, QJsonArray({1, true})};
        QString result = tcpClient(shared->manager_host, 7000, jats({5, send}));

        log->code_editor->setPlainText(result);
        ui->log->show();
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

    for (auto item : ui->servers->selectedItems())
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
    auto selected = ui->servers->selectedItems();
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

void servers_class::message_action()
{
}

QString servers_class::to_action(QString action, QString info)
{

    QJsonArray pks;
    auto selected = ui->servers->selectedItems();
    for (auto item : selected)
    {
        QString server_name = item->text(0);
        if (not item->parent())
        {
            pks.push_back({{server_name, action, info}});
        }
    }
    pks = {"serverAction", pks};

    QString recv = tcpClient(shared->manager_host, 7000, jats({3, pks}));

    return recv;
}

void servers_class::send_to_vserver(QString action, QString info)
{

    QList<QTreeWidgetItem *> selected = ui->servers->selectedItems();

    for (auto item : selected)
    {
        QString host = item->text(7);

        QJsonArray send = {host, QJsonArray({4, {{action, info}}})};
        tcpClient(shared->manager_host, 7000, jats({5, send}));
    }
    //-------------------------------------
}