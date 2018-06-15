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
#include "json.h"
#include <QDebug>
#include <QString>

using namespace std;
using namespace nlohmann;

string replace(string frase, string src, string dst);
string between(string frase, string before, string after);
string fread(string path);
void fwrite(string path, string data);
void awrite( string path, string data );
string timeStruct(float t);
vector <string> split( string frase, string separator );
void sorted( vector <string>& _vector );
bool in_vector( string word, vector <string> _vector );
bool in_string( string word, string _string );
const string currentDateTime( int num );
const string secToTime( float sec );
json jread( string path );
void jwrite( string path, json data );
void debug( QString bug );
void print( string input );
string path();

#endif //UTIL_h