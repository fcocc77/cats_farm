#include "submit.hpp"

void submit::init(int argc, char const *argv[])
{

	QString arg, jobName, server, serverGroup, firstFrame, lastFrame, taskSize, priority,
		suspend, comment, software, project, extra, system, instances, render, ip;

	for (int i = 1; i < argc; i++)
	{
		arg = argv[i];
		if (arg == "-jobName")
			jobName = argv[i + 1];
		if (arg == "-server")
			server = argv[i + 1];
		if (arg == "-serverGroup")
			serverGroup = argv[i + 1];
		if (arg == "-firstFrame")
			firstFrame = argv[i + 1];
		if (arg == "-lastFrame")
			lastFrame = argv[i + 1];
		if (arg == "-taskSize")
			taskSize = argv[i + 1];
		if (arg == "-priority")
			priority = argv[i + 1];
		if (arg == "-suspend")
			suspend = argv[i + 1];
		if (arg == "-comment")
			comment = argv[i + 1];
		if (arg == "-software")
			software = argv[i + 1];
		if (arg == "-project")
			project = argv[i + 1];
		if (arg == "-extra")
			extra = argv[i + 1];
		if (arg == "-system")
			system = argv[i + 1];
		if (arg == "-instances")
			instances = argv[i + 1];
		if (arg == "-render")
			render = argv[i + 1];
		if (arg == "-ip")
			ip = argv[i + 1];
	}

	bool _suspend = false;
	if (suspend == "1")
		_suspend = true;

	QJsonArray info = {jobName, server, serverGroup, firstFrame.toInt(), lastFrame.toInt(), taskSize.toInt(), priority.toInt(),
					   _suspend, comment, software, project, extra, system, instances.toInt(), render};

	QJsonObject settings = jread(path + "/etc/settings.json");
	int port = settings["manager"].toObject()["port"].toInt();
	QString host = settings["manager"].toObject()["host"].toString();

	if (!ip.isEmpty())
		host = ip;

	if (argc > 2)
		tcpClient(host, port, jats({4, info}));
}
