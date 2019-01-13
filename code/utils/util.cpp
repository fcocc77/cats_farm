
//librereias propias
#include "util.h"
//------------------------

QString fread(QString _path)
{
	QFile file(_path);
	file.open(QIODevice::ReadOnly);

	QTextStream in(&file);
	QString info;
	while (!in.atEnd())
		info += in.readLine() + "\n";
	file.close();

	return info.left(info.size() - 1); // borra el ultimo caracter "\n"
}

void fwrite(QString _path, QString data)
{
	const QString qPath(_path);
	QFile qFile(qPath);
	if (qFile.open(QIODevice::WriteOnly))
	{
		QTextStream out(&qFile);
		out << data;
		qFile.close();
	}
}

void awrite(QString _path, QString data)
{
	fwrite(_path, fread(_path) + data);
}

QJsonObject jread(QString _path)
{
	QJsonDocument _json = QJsonDocument::fromJson(fread(_path).toUtf8());
	return _json.object();
}

void jwrite(QString _path, QJsonObject data)
{
	QJsonDocument doc(data);
	fwrite(_path + ".tmp", doc.toJson(QJsonDocument::Indented));

	os::remove(_path + ".old");
	os::rename(_path, _path + ".old");
	os::rename(_path + ".tmp", _path);
	os::remove(_path + ".tmp");
}

QString jats(QJsonArray data)
{ // jats =  json array to string
	QJsonDocument doc(data);
	return doc.toJson(QJsonDocument::Compact);
}

QString jots(QJsonObject data)
{ // jots =  json object to string
	QJsonDocument doc(data);
	return doc.toJson(QJsonDocument::Compact);
}

QJsonObject jofs(QString data)
{ // jofs =  json object from string
	return QJsonDocument::fromJson(data.toUtf8()).object();
}
QJsonArray jafs(QString data)
{ // jafs =  json array from string
	return QJsonDocument::fromJson(data.toUtf8()).array();
}

QString timeStruct(float t)
{
	float _h, _m;
	int h, m, s;

	_h = t / 60 / 60;
	h = _h;

	_m = (_h - h) * 60;
	m = _m;

	s = (_m - m) * 60;

	QString hour = "0" + QString::number(h),
			minute = "0" + QString::number(m),
			second = "0" + QString::number(s);

	return hour.right(2) + ":" + minute.right(2) + ":" + second.right(2);
}

const QString secToTime(float sec)
{
	float f_day = 0, f_hour = 0, f_min = 0;
	int i_day, i_hour, i_min, i_sec;
	QString s_day, s_hour, s_min, s_sec;

	//si es que la estructura tieme mas de un dijito, se suma un cero antes
	auto to_two = [](QString &dig) { if ( dig.size() == 1 ){ dig = "0" + dig; } };
	//---------------------------------------------------------------------

	f_day = ((sec / 60) / 60) / 24;
	i_day = f_day;
	s_day = QString::number(i_day);

	f_hour = (f_day - i_day) * 24;
	i_hour = f_hour;
	s_hour = QString::number(i_hour);
	to_two(s_hour);

	f_min = (f_hour - i_hour) * 60;
	i_min = f_min;
	s_min = QString::number(i_min);
	to_two(s_min);

	i_sec = (f_min - i_min) * 60;
	s_sec = QString::number(i_sec);
	to_two(s_sec);

	QString _time;
	if (i_day)
	{
		if (i_day > 1)
		{
			_time = s_day + " days, " + s_hour + ":" + s_min + ":" + s_sec;
		}
		else
		{
			_time = s_day + " day, " + s_hour + ":" + s_min + ":" + s_sec;
		}
	}
	else
	{
		_time = s_hour + ":" + s_min + ":" + s_sec;
	}

	return _time;
}

const QString currentDateTime(int num)
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);

	if (num == 0)
	{
		strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
	}
	else if (num == 1)
	{
		strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
	}
	else if (num == 2)
	{
		strftime(buf, sizeof(buf), "%H:%M:%S", &tstruct);
	}
	else if (num == 3)
	{
		strftime(buf, sizeof(buf), "%Y", &tstruct);
	}
	else if (num == 4)
	{
		strftime(buf, sizeof(buf), "%m", &tstruct);
	}
	else if (num == 5)
	{
		strftime(buf, sizeof(buf), "%d", &tstruct);
	}
	else if (num == 6)
	{
		strftime(buf, sizeof(buf), "%H", &tstruct);
	}
	else if (num == 7)
	{
		strftime(buf, sizeof(buf), "%M", &tstruct);
	}
	else
	{
		strftime(buf, sizeof(buf), "%S", &tstruct);
	}

	return buf;
}

void print(QString input)
{
	qDebug().nospace().noquote() << input;
}

QString getPath()
{
	if (_win32)
		return "C:/cats_farm";
	else if (_darwin)
		return "/usr/local/cats_farm";
	else
		return "/opt/cats_farm";
}

/*

void sorted( vector <QString>& _vector ){
	sort( _vector.begin(), _vector.end() );
}

bool in_vector( QString word, vector <QString> _vector ){ 
	if ( find(_vector.begin(), _vector.end(), word) == _vector.end() ){ return false; }
	else { return true; }
}

*/