#ifndef OS_HPP
#define OS_HPP

#include <iostream>
using namespace std;
#include <math.h>   /* round, floor, ceil, trunc */
#include <unistd.h> // sleep usleep
#include <fstream>  // ifstream
#include <QHostInfo>
#include <QString>
#include <QDebug>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QDirIterator>
#include <QDir>
#include <QFile>
#include <QProcess>
//librerias propias
#include "util.hpp"
//--------------------

#ifdef _WIN32
#include <windows.h>
#include <TlHelp32.h>
#endif

namespace os
{

void copy(QString src, QString dst);
void copydir(QString src, QString dst);
void move(QString src, QString dst);
void remove(QString _file);
void system(QString cmd);
int get_cpu_used();
int get_iowait_cpu_used();
int get_process_cpu_used(int pid);
int get_ram_percent(bool cached_percent = false);
float get_ram_total();
float get_ram_used();
int get_cpu_temp();
int get_cpu_cores();
QList<long> get_cpu_stat();
QList<float> ram();
QString dirname(QString file);
QString basename(QString file);
bool isfile(QString file);
bool isdir(QString dir);
void makedirs(QString dir);
void kill(int pid, bool force = false);
QStringList listdir(QString folder, bool onlyname = 0);
QString sh(QString cmd, bool error = true);
const QString hostName();
const QString ip();
const QString mac();
const QString user();
void back(QString cmd);
void rename(QString src, QString dst);
void copymove(QString src, QString dst, bool copy);
} // namespace os

//define el systema operativo
#ifdef _WIN32
#define _win32 1
#define _darwin 0
#define _linux 0
#elif __APPLE__
#define _win32 0
#define _darwin 1
#define _linux 0
#elif __linux__
#define _win32 0
#define _darwin 0
#define _linux 1
#else
#define _win32 0
#define _darwin 0
#define _linux 0
#endif
//----------------------------

#endif //OS_HPP