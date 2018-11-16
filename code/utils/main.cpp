// #include "../utils/util.h"

#include <QDebug>
#include <QString>
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QMessageBox>


void fwrite( QString path, QString data ){
	const QString qPath( path );
	QFile qFile(qPath);
	if (qFile.open(QIODevice::WriteOnly)) {
		QTextStream out(&qFile); out << data;
		qFile.close();
	}
}



int main(){

	QString _file = "C:/Users/jumpcats-01/Desktop/log.cpp";

	fwrite( _file, "holas \ntest\n asdasdiasdi");
}