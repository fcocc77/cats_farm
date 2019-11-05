#include "server.hpp"

server::server()
{
	QStringList manager_hosts = fread(path + "/etc/manager_host").split(",");
	QString host = manager_hosts[0];
	
	render = new render_class(&mutex);

	tcpClient(host, 7000, &server::send_resources, this);
	tcpServer(7001, &server::recieveManager, this);
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
	{
		system = "Linux";
	}
	else if (_win32)
		system = "Windows";
	else
	{
		system = "Mac";
	}

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

	QJsonArray server_info = {os::hostName(),
							  os::ip(),
							  os::cpuUsed(),
							  os::ramPercent(),
							  os::cpuTemp(),
							  system,
							  render->vbox_working(),
							  os::ramTotal(),
							  os::ramUsed(),
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
		{
			if (_linux)
			{
				os::sh("sync && sysctl -w vm.drop_caches=3");
			}
		}

		if (action == "vmstart")
		{
			QString vm;
			if (_linux)
			{
				vm = "'VBoxManage startvm win2016 --type headless'";
				vm = "runuser -l " + os::user() + " -c " + vm;
			}
			else if (_win32)
			{
				vm = "\"C:/Program Files/Oracle/VirtualBox/VBoxManage.exe\" startvm win2016 --type headless";
			}

			os::sh(vm);
		}

		if (action == "vmstop")
		{

			QString vm;
			if (_linux)
			{
				vm = "'VBoxManage controlvm win2016 savestate'";
				vm = "runuser -l " + os::user() + " -c " + vm;
			}
			else if (_win32)
			{
				vm = "\"C:/Program Files/Oracle/VirtualBox/VBoxManage.exe\" controlvm win2016 savestate";
			}

			os::sh(vm);
			fwrite(path + "/log/vbox", "0");
		}

		if (action == "cpu_limit")
		{
			fwrite(path + "/etc/cpu_limit", info);
		}
	}

	return send;
}
