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

//librerias propias
#include "util.h"
//--------------------

#ifdef _WIN32
	#include <windows.h>
#endif


class os{
public:

	static void mkdir( string path );
	static void copy( string src, string dst );
	static void move( string src, string dst );	
	static void remove( string _file );
	static void system( string cmd );
	static int cpuUsed();
	static int ramPercent();
	static int ramTotal();
	static float ramUsed();
	static int cpuTemp();
	static int cpuCount();
	static vector <int> getStat();
	static vector <float> ram();
	static string dirname (string file);
	static string basename(string file);
	static bool isfile( string file );
	static bool isdir( string dir );
	static void kill( int pid );
	static vector <string> listdir( string folder, bool onlyname = 0 );
	static string sh(string cmd);
	static const string hostName();
	static const string ip();
	static const string user();
	static void back( string cmd );
	static void rename( string src, string dst );

private:
	static void copymove( string src, string dst, bool copy );

};

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