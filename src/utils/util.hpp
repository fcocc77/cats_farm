#ifndef UTIL_HPP
#define UTIL_HPP
#include <iostream>
#include <fstream>   // ifstream
#include <vector>    //
#include <sstream>   // istringstream
#include <algorithm> //sort , find
#include <typeinfo>
#include "os.hpp"
#include <ctime> // time_t
#include <QDebug>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QMessageBox>
#include <QList>
#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

using namespace std;

QString fread(QString path);
void fwrite(QString path, QString data);
void awrite(QString path, QString data);
QString timeStruct(float t);
void sorted(vector<QString> &_vector);
bool in_vector(QString word, vector<QString> _vector);
const QString currentDateTime(int num);
const QString secToTime(float sec);
QJsonObject jread(QString path);
void jwrite(QString path, QJsonObject data);
QString getPath();
QString jats(QJsonArray data);  // jats =  json array to string
QString jots(QJsonObject data); // jots =  json object to string
QJsonObject jofs(QString data); // jofs =  json object from string
QJsonArray jafs(QString data);  // jafs =  json array from string
QLayout *qlayout(QLayout *parent, QString direction, QString name = "");

// print para 1, 2 y 3 argumentos
template <class T>
void print(T input)
{
    qDebug().nospace().noquote() << input;
}

template <class T1, class T2>
void print(T1 input1, T2 input2)
{
    qDebug().nospace().noquote() << input1 << " " << input2;
}

template <class T1, class T2, class T3>
void print(T1 input1, T2 input2, T3 input3)
{
    qDebug().nospace().noquote() << input1 << " " << input2 << " " << input3;
}
// ----------------------------

const QString path = getPath();

#endif //UTIL_HPP