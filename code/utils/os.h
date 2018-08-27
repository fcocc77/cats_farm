#ifndef OS_H
#define OS_H

#include <iostream>
using namespace std;
#include <math.h>       /* round, floor, ceil, trunc */
#include <unistd.h>// sleep usleep
#include <fstream> // ifstream
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
#include "util.h"
//--------------------

#ifdef _WIN32
	#include <windows.h>
	#include <TlHelp32.h>
#endif

namespace os {
	
	void mkdir( string path );
	void copy( string src, string dst );
	void move( string src, string dst );	
	void remove( string _file );
	void system( string cmd );
	int cpuUsed();
	int ramPercent();
	int ramTotal();
	float ramUsed();
	int cpuTemp();
	int cpuCount();
	vector <int> getStat();
	vector <float> ram();
	string dirname (string file);
	string basename(string file);
	bool isfile( string file );
	bool isdir( string dir );
	void kill( int pid );
	vector <string> listdir( string folder, bool onlyname = 0 );
	string sh(string cmd);
	const string hostName();
	const string ip();
	const string user();
	void back( string cmd );
	void rename( string src, string dst );
	string qp( string cmd );
	void copymove( string src, string dst, bool copy );
}

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

#endif //OS_H