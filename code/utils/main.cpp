#include "../utils/os.h"
#include "../utils/util.h"
#include "../utils/tcp.h"

#include <QDebug>
#include <QString>
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>

int main(){

	QJsonObject array = {{ "casa","hola" }};

	array["casa"] = "test";
	array["ki"] = "lala";



	QJsonDocument doc;
	doc.setObject(array);

	QString strJson(doc.toJson(QJsonDocument::Compact));
	
	qDebug() <<  strJson;
 
}