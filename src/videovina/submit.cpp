#include "submit.h"

void submit::init(int argc, char const *argv[])
{
	QString ip, user, project;

	ip = argv[1];
	user = argv[2];
	project = argv[3];

	QJsonArray info = {ip, user, project};

	if (argc > 2)
		tcpClient(ip, 7000, jats({7, info}));
	else
		qDebug() << "Without arguments.";
}
