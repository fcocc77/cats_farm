#include "submit.h"
#include "../global/global.h"

void submit::init(int argc, char const *argv[])
{
	QString ip, user, user_id, type, name, format;

	ip = argv[1];
	user = argv[2];
	user_id = argv[3];
	type = argv[4];
	name = argv[5];
	format = argv[6];

	QJsonArray info = {user, user_id, type, name, format};
	QJsonObject settings = jread(VINARENDER_CONF_PATH + "/settings.json");
	int port = settings["manager"].toObject()["port"].toInt();

	if (argc > 2)
		tcpClient(ip, port, jats({7, info}));
	else
		qDebug() << "Without arguments.";
}
