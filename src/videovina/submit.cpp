#include "submit.h"

void submit::init(int argc, char const *argv[])
{
	QString ip, user, type, name, proxy;

	ip = argv[1];
	user = argv[2];
	type = argv[3];
	name = argv[4];
	proxy = argv[5];

	QJsonArray info = {user, type, name, proxy};

	if (argc > 2)
		tcpClient(ip, 7000, jats({7, info}));
	else
		qDebug() << "Without arguments.";
}
