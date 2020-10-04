#include "server.hpp"

server::server()
{
	render = new render_class(&mutex);

	// obtiene los puertos del manager y server
	QJsonObject settings = jread(path + "/etc/settings.json");
	QString manager_host = settings["manager"].toObject()["ip"].toString();
	int manager_port = settings["manager"].toObject()["port"].toInt();
	int server_port = settings["server"].toObject()["port"].toInt();
	// -------------------------------

	tcpClient(manager_host, manager_port, &server::send_resources, this);
	tcpServer(server_port, &server::recieveManager, this);
}

QString server::send_resources(QString recv, QJsonObject extra)
{
	if (not recv.isEmpty())
	{
		mutex.lock();
		render->preferences = jofs(recv);
		mutex.unlock();
		jwrite(path + "/etc/preferences_s.json", render->preferences);
	}

	QString system;
	if (_linux)
		system = "Linux";
	else if (_win32)
		system = "Windows";
	else
		system = "Mac";

	//get ssh user
	bool usr = false;
	static QString username;
	static QString userpass;

	if (not usr)
	{
		if (_win32)
		{
			username = os::sh("echo %username%").split(" ")[0];
			userpass = fread("C:/ProgramData/vinarender/user");
		}
		else
		{
			username = "root";
			userpass = "vfx";
		}
		usr = true;
	} //------------------------------------------

	QJsonArray server_info = {
		os::hostName(),
		os::ip(),
		os::cpuUsed(),
		os::get_ram_percent(),
		os::get_ram_percent(true),
		os::cpuTemp(),
		system,
		os::mac(),
		os::get_ram_total(),
		os::get_ram_used(),
		os::cpuCount(),
		"log",
		username,
		userpass};

	return jats({1, server_info});
}

QString server::recieveManager(QString _recv)
{

	QJsonArray json = jafs(_recv);
	int input = json[0].toInt();
	QJsonArray recv = json[1].toArray();

	QString send;

	if (input == 0)
		send = render->render_task(recv);

	if (input == 1)
	{
		bool failed = recv[0].toBool();
		if (failed)
			send = fread(path + "/log/render_log");
		else
			send = fread(path + "/log/render_log_0");
	}

	if (input == 3)
	{
		mutex.lock();
		for (auto i : recv)
		{
			int pid = render->pid[i.toInt()];
			if (pid)
			{
				render->taskKill[i.toInt()] = true;
				os::kill(pid);
			}
		}
		mutex.unlock();

		// Mata las pid a partir del nombre "d" (es el after effect render de wine)
		QStringList ps = os::sh("ps cax").split("\n");
		for (QString line : ps)
		{
			QString lastWord = line.split(" ").back();
			if (lastWord == "d")
			{
				QString pid = line.split(" ")[0];
				os::sh("kill " + pid);
			}
		}
		// -------------------------------------------
	}

	if (input == 4)
	{
		QString action = recv[0].toString();
		QString info = recv[1].toString();

		if (action == "freeram")
			if (_linux)
				os::sh("sh -c \"sync && sysctl -w vm.drop_caches=3\"");
		if (action == "off")
			os::system("shutdown now");
	}

	return send;
}
