#include "servers_actions.h"

// Server Funcions

void servers_actions::init()
{
	actions();
}

void servers_actions::actions()
{

	// Server List Connections

	connect(serverList, &QTreeWidget::customContextMenuRequested, this, &servers_actions::server_popup);

	// Server Action

	connect(deleteAction, &QAction::triggered, this, [this]() {
		QString ask = "Are you sure to want delete this server?";
		QString tile = "Server Delete";
		QString action = "delete";

		serverMessage(&servers_actions::serverAction, action, ask, tile, "None", this);
	});

	connect(serverInactiveAction, &QAction::triggered, this, [this]() {
		QString ask = "The task will kill, Sure you want to disable this server?";
		QString tile = "Server Disabling";
		QString action = "inactive";
		serverMessage(&servers_actions::serverAction, action, ask, tile, "None", this);
	});
	connect(serverReactiveAction, &QAction::triggered, this, [this]() {
		serverAction("reactive", "None");
	});
	connect(serverMaxInstancesAction, &QAction::triggered, this, &servers_actions::serverMaxInstances);
	connect(serverFreeramAction, &QAction::triggered, this, [this]() {
		sendToServer("freeram", "none");
	});
	connect(serverSshAction, &QAction::triggered, this, &servers_actions::serverSSH);
	connect(serverShowLog, &QAction::triggered, this, &servers_actions::serverLog);
	serverShowLog->setShortcut(QString("Ctrl+L"));
	connect(serverVncAction, &QAction::triggered, this, &servers_actions::serverVNC);
	//--------------------------------------------------------
}

void servers_actions::serverOptions()
{

	uiServerOptions->show();

	// Recive los servers del jobs que estan en el manager

	QList<QTreeWidgetItem *> selected = serverList->selectedItems();

	firstServerItem = selected[0];

	if (not selected.empty())
	{

		QJsonArray pks = {(selected[0]->text(0)), "None", "read"};
		pks = {"serverOptions", pks};
		QString schedule = tcpClient(managerHost, 7000, jats({3, pks}));

		QStringList range = schedule.split("-");

		uiServerOptions->schedule_start->setCurrentIndex(range[0].toInt());
		uiServerOptions->schedule_end->setCurrentIndex(range[1].toInt());
	}
}

void servers_actions::serverOptionsSend()
{

	QString schedule = uiServerOptions->schedule_start->currentIndex() + "-" + uiServerOptions->schedule_end->currentIndex();

	QJsonArray pks;
	auto selected = serverList->selectedItems();
	selected.push_back(firstServerItem);

	for (auto item : selected)
	{
		QString server = item->text(0);
		pks.push_back({{server, schedule, "write"}});
	}

	pks = {"serverOptions", pks};
	tcpClient(managerHost, 7000, jats({3, pks}));
}

void servers_actions::server_popup()
{
	auto selected = serverList->selectedItems();

	QMenu *menu = new QMenu(monitor);
	if (not selected.empty())
	{

		menu->addAction(serverInactiveAction);
		menu->addAction(serverReactiveAction);
		menu->addAction(deleteAction);

		menu->addSeparator();
		menu->addAction(serverShowLog);
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
		connect(mapper, SIGNAL(mapped(int)), this, SLOT(serverMaxInstances(int)));

		menu->addMenu(submenu);
		//-------------------------------------------------

		menu->addAction(serverFreeramAction);
		menu->addSeparator();
		menu->addAction(serverSshAction);
		menu->addAction(serverVncAction);
		menu->addSeparator();
		menu->popup(QCursor::pos());
	}
	else
	{

		menu->addAction(serverList->showAllAction);
		menu->addAction(serverList->hideAllAction);
		menu->addSeparator();
		menu->addAction(serverList->displayOnAction);
		menu->addAction(serverList->displayOffAction);
		menu->addSeparator();
		menu->addAction(serverList->displayWindowsAction);
		menu->addAction(serverList->displayLinuxAction);
		menu->addAction(serverList->displayMacAction);
		menu->popup(QCursor::pos());
	}
}

void servers_actions::serverCpuLimit(int limit)
{
	sendToServer("cpu_limit", QString::number(limit));
}

void servers_actions::serverLog()
{

	auto selected = serverList->selectedItems();
	if (not selected.empty())
	{
		QString host = (selected[0]->text(7));

		QJsonArray send = {host, QJsonArray({1, true})};
		QString result = tcpClient(managerHost, 7000, jats({5, send}));

		log_text->setPlainText(result);
		log_dock->show();
	}
}

void servers_actions::serverMaxInstances(int ins)
{
	serverAction("max_instances", QString::number(ins));
}

void servers_actions::serverSSH()
{

	QJsonArray recv = jafs(serverAction("ssh", "None"));

	QString sshUser = recv[0].toString();
	QString sshPass = recv[1].toString();
	QString ip = recv[2].toString();

	QString disable_ask = " -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no ";
	QString cmd;
	QString ssh = "C:/cats_farm/os/win/OpenSSH/bin/ssh.exe";
	QString linux_sshpass = "sshpass -p " + sshPass + " ssh -o StrictHostKeyChecking=no " + sshUser + "@" + ip;

	if (_win32)
		cmd = "cmd.exe /K start cmd.exe /K " + ssh + " -tt " + disable_ask + sshUser + "@" + managerHost + " \"" + linux_sshpass + "\"";
	else
		cmd = "gnome-terminal -e '" + linux_sshpass + "'";
	os::back(cmd);
}

void servers_actions::serverVNC()
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

	for (auto item : serverList->selectedItems())
	{
		QString ip = item->text(7);
		QString arg = "\"" + tigervnc + "\" " + ip + ":7";
		os::back(arg);
	}
}

void servers_actions::serverMessage(QString (servers_actions::*funtion)(QString, QString), QString action,
									QString ask, QString tile, QString info, servers_actions *_class)
{

	auto selected = serverList->selectedItems();
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

void servers_actions::serverActionMessage()
{
}

QString servers_actions::serverAction(QString action, QString info)
{

	QJsonArray pks;
	auto selected = serverList->selectedItems();
	for (auto item : selected)
	{
		QString server_name = item->text(0);
		if (not item->parent())
		{
			pks.push_back({{server_name, action, info}});
		}
	}
	pks = {"serverAction", pks};

	QString recv = tcpClient(managerHost, 7000, jats({3, pks}));

	return recv;
}

void servers_actions::sendToServer(QString action, QString info)
{

	QList<QTreeWidgetItem *> selected = serverList->selectedItems();

	for (auto item : selected)
	{
		QString host = item->text(7);

		QJsonArray send = {host, QJsonArray({4, {{action, info}}})};
		tcpClient(managerHost, 7000, jats({5, send}));
	}
	//-------------------------------------
}
//----------------------------------