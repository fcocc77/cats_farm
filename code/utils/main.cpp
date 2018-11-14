#include "../utils/os.h"
#include "../utils/util.h"
#include "../utils/tcp.h"

#include <QDebug>
#include <QString>
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>

int main(){

	QJsonObject info;
	info[ "jobs" ];


	info[ "jobs" ] = QJsonObject({{"Casa",7}, {"tet",8} });

	info["jobs"].toObject()["Casa"] = 10;
	qDebug() << info;

	qDebug() << info["jobs"].toObject()["Casa"].toInt();

 
}