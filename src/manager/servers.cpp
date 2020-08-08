#include "manager.hpp"

QString manager::server_action(QJsonArray pks)
{

	for (QJsonValue s : pks)
	{
		QJsonArray _server = s.toArray();
		QString name = _server[0].toString();
		QString server_action = _server[1].toString();
		int instances = _server[2].toString().toInt();
		auto server = get_server(name);

		if (server_action == "turn_on")
			os::sh("ether-wake " + server->mac);

		if (server_action == "max_instances")
			server->max_instances = instances;

		if (server_action == "inactive")
			server_set_state(server, false);

		if (server_action == "reactive")
			server_set_state(server, true);

		if (server_action == "delete")
			if (not(server->status == "active"))
				erase_by_name(servers, name);

		if (server_action == "ssh")
			return jats({ server->sshUser, server->sshPass, server->host });
	}

	return "{}";
}

void manager::server_set_state(server_struct *server, bool state)
{

	if (state)
	{
		for (int i = 0; i < server->max_instances; ++i)
		{
			auto ins = server->instances[i];
			if (ins->status == 2)
			{
				ins->status = 0;
				ins->job_task = "...";
			}
		}
	}

	else
	{
		QJsonArray kill_ins;

		for (int i = 0; i < server->max_instances; ++i)
		{
			auto ins = server->instances[i];
			ins->status = 2;
			ins->job_task = "...";
			kill_ins.push_back(i);
		}

		tcpClient(server->host, server_port, jats({ 3, kill_ins }));
	}
}

server_struct *manager::get_server(QString name)
{
	for (auto server : servers)
		if (server->name == name)
			return server;
	return servers[0];
}

QString manager::server_options(QJsonArray pks)
{

	for (QJsonValue s : pks)
	{
		QJsonArray _server = s.toArray();

		QString name = _server[0].toString();
		QJsonArray recv = _server[1].toArray();
		QString action = _server[2].toString();

		auto server = get_server(name);

		if (action == "read")
		{
			return { server->schedule };
		}

		if (action == "write")
		{
			server->schedule = recv[0].toString();
		}
	}

	return "";
}
