#include "../utils/util.h"
#include "../utils/os.h"
#include "../utils/tcp.h"
#include "../utils/video.h"



#include <QDebug>
#include <QString>
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QMessageBox>
#include <QJsonArray>







int main(){

	QJsonArray test = {10,20,49};


	for ( auto i : test ){
		qDebug() << i;
	}

}