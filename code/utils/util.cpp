
//librereias propias
#include "util.h"
//------------------------

//funcion para remplazar una palabra por otra
string replace( string frase, string src, string dst ){
    if ( not src.empty() ){ 
        int pos = frase.find(src);
        while (pos != -1){
            frase.replace(pos, src.size(), dst);
            pos = frase.find(src, pos + dst.size());
        }
    }

    return frase;
}//-----------------------------------------------

string between( string frase, string before, string after ){
    int pos = frase.find(before);
    frase=frase.erase(0,pos+before.size());

    if (after.size()){
        pos = frase.find(after);
        frase=frase.substr(0,pos);
    }
    return frase;
}

string fread( string path ){
    string output;

    ifstream input(path);

    for(string line; getline( input, line ); ){
        output+=line+"\n";
    }

    if ( not output.empty() ) output.erase( output.size() - 1); // borra el ultimo \n
    return output;
}

void fwrite( string path, string data ){
    ofstream myfile( path );
    myfile << data;
    myfile.close();
}

void awrite( string path, string data ){
    ofstream myfile( path, ios::app );
    myfile << data;
    myfile.close();
}

json jread( string path ){
    // cuando la estructura de json esta mal da error Runtime, intenta para evitarlo
    try { return json::parse( fread( path ) ); }
    catch( exception& e ){
        try { return json::parse( fread( path + ".old" ) ); }
        catch( exception& e ){}
    };

    return {};
    //-----------------------------------
}

void jwrite( string path, json data ){
    fwrite( path + ".tmp", data.dump(4) );
    os::remove( path + ".old" );
    os::rename( path, path + ".old" );
    os::rename( path + ".tmp", path );
    os::remove( path + ".tmp" );
}

string timeStruct(float t){
    float _h, _m;
    int h, m ,s;

    _h = t/60/60;
    h = _h;

    _m = (_h - h)*60;
    m = _m;

    s = (_m - m)*60;

    string hour = "0" + to_string(h),
            minute = "0" + to_string(m), 
            second = "0" + to_string(s);

    return hour.substr(hour.length()-2) +":"
           +minute.substr(minute.length()-2) +":"
           +second.substr(second.length()-2);
}

vector <string> split( string frase, string separator ){

    vector <string> list;

    int pos = frase.find(separator);
    int start = 0;
    while (pos != -1){
        list.push_back( frase.substr(start,pos-start)  );
        start = pos + separator.size();
        pos = frase.find(separator, start);
    }

    // agrega el ultimo item
    list.push_back( frase.substr( start, frase.size() - start )  );
    //--------------------------------------

    return list;
}

void sorted( vector <string>& _vector ){
    sort( _vector.begin(), _vector.end() );
}

bool in_vector( string word, vector <string> _vector ){ 
    if ( find(_vector.begin(), _vector.end(), word) == _vector.end() ){ return false; }
    else { return true; }
}

bool in_string( string word, string _string ){  
    if ( not ( _string.find( word ) == string::npos ) ) { return true; }
    else{ return false; }   
}

const string currentDateTime( int num ) {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    if ( num == 0 ){ strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct); }
    else if( num == 1 ){ strftime(buf, sizeof(buf),  "%Y-%m-%d", &tstruct); } 
    else if( num == 2 ){ strftime(buf, sizeof(buf),  "%H:%M:%S", &tstruct); } 
    else if( num == 3 ){ strftime(buf, sizeof(buf), "%Y", &tstruct); } 
    else if( num == 4 ){ strftime(buf, sizeof(buf), "%m", &tstruct); } 
    else if( num == 5 ){ strftime(buf, sizeof(buf), "%d", &tstruct); } 
    else if( num == 6 ){ strftime(buf, sizeof(buf), "%H", &tstruct); } 
    else if( num == 7 ){ strftime(buf, sizeof(buf), "%M", &tstruct); } 
    else{ strftime(buf, sizeof(buf), "%S", &tstruct); }

    return buf;
}

const string secToTime( float sec ){
    float f_day = 0, f_hour = 0, f_min = 0;
    int i_day, i_hour, i_min, i_sec;
    string s_day, s_hour, s_min, s_sec;

    //si es que la estructura tieme mas de un dijito, se suma un cero antes
    auto to_two = []( string& dig ){ if ( dig.size() == 1 ){ dig = "0" + dig; } };
    //---------------------------------------------------------------------

    f_day = ( ( sec / 60 ) / 60 ) / 24;
    i_day = f_day;
    s_day = to_string(i_day);

    f_hour = ( f_day - i_day ) * 24;
    i_hour = f_hour;
    s_hour = to_string( i_hour );
    to_two( s_hour );

    f_min = ( f_hour - i_hour ) * 60;
    i_min = f_min;
    s_min = to_string( i_min );
    to_two( s_min );

    i_sec = ( f_min - i_min ) * 60;
    s_sec = to_string( i_sec );
    to_two( s_sec );

    string _time; 
    if ( i_day ){
        if ( i_day > 1 ){ _time = s_day + " days, "+ s_hour + ":" + s_min + ":" + s_sec; }
        else{ _time = s_day + " day, "+ s_hour + ":" + s_min + ":" + s_sec; }
    }
    else{ _time = s_hour + ":" + s_min + ":" + s_sec; }

    return _time;
}

void debug( QString bug ){

    /*
    static ofstream file( "../../log/monitor.log" , ios::app);

    static int lines = 0;
    lines++;
    if ( lines > 500 ){
        file << bug + "\n";
        lines = 0;
    }
    else os::remove("../../log/monitor.log" );
    file.close();
    */
    //qDebug() << bug;
}

void print( string input ){ 
     qDebug() << QString::fromStdString(input);
}

string path(){

    json paths = jread( "../../etc/paths.json" );

    if ( _win32 ) return paths["win"];
    else if ( _darwin ) return paths["mac"];
    else return paths["linux"];
}