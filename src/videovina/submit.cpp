#include "submit.h"

void submit::init(int argc, char const *argv[])
{
	QString ip, user, user_id, type, name, proxy;

	ip = argv[1];
	user = argv[2];
	user_id = argv[3];
	type = argv[4];
	name = argv[5];
	proxy = argv[6];

	QJsonArray info = {user, user_id, type, name, proxy};

	if (argc > 2)
		tcpClient(ip, 7000, jats({7, info}));
	else
		qDebug() << "Without arguments.";
}
