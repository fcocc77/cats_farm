#include "../utils/os.h"
#include "../utils/util.h"
#include "../utils/tcp.h"

#include <QDebug>
#include <QString>
#include <QObject>
#include <QJsonObject>

int main(){

	QJsonObject array = {{ "casa","hola" }};

	array["casa"] = "test";
	array["ki"] = "lala";


	qDebug() <<  array["ki"].toString();
 
}