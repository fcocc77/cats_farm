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


	info = {{"casa",10},{"otro","20"}};

	for (auto aa : info )
		qDebug() << aa.toInt();
 
}