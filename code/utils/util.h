#ifndef UTIL_h
#define UTIL_h
#include <iostream>
#include <fstream> // ifstream
#include <vector> // 
#include <sstream> // istringstream
#include <algorithm> //sort , find
#include <typeinfo>
#include "os.h"
#include <ctime> // time_t
#include <QDebug>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QMessageBox>

using namespace std;


QString fread(QString path);
void fwrite(QString path, QString data);
void awrite( QString path, QString data );
QString timeStruct(float t);
void sorted( vector <QString>& _vector );
bool in_vector( QString word, vector <QString> _vector );
const QString currentDateTime( int num );
const QString secToTime( float sec );
QJsonObject jread( QString path );
void jwrite( QString path, QJsonObject data );
void print( QString input );
QString getPath();
QString jats( QJsonArray data );  // jats =  json array to string
QString jots(  QJsonObject data ); // jots =  json object to string
QJsonObject jofs( QString data ); // jofs =  json object from string
QJsonArray jafs( QString data ); // jafs =  json array from string


const QString path = getPath();

#endif //UTIL_h