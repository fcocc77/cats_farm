#include "../utils/util.h"
#include "../utils/os.h"
#include "../utils/tcp.h"
#include "../utils/video.h"
#include "../utils/threading.h"

#include <QDebug>
#include <QString>
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QMessageBox>
#include <QJsonArray>
#include <QList>
#include <unistd.h> // sleep usleep

QJsonObject data;
QJsonObject data2;
void test_funccion()
{
	while (1)
	{
		data = {
			{"property1", 1},
			{"property2", 2}};

		for (size_t i = 0; i < 10000; i++)
		{
			data["test" + QString::number(i)] = 20;
		}
	}
}

int main()
{

	threading(&test_funccion);

	while (1)
	{

		QJsonObject casa;

		casa = data;

		qDebug() << data.empty();
		QJsonDocument doc(casa);

		qDebug() << data.size();
		usleep(100000);
	}
}
