#include <iostream>

using namespace std;

#include "util.h"
#include "os.h"
void concat(string folder){

	string ffmpeg, logMetod, dirMovie, list, safe, concat, movie, movie_list, name, cmd, null;

	if ( _win32 ){
		ffmpeg = "C:/CatsFarm/os/win/ffmpeg/ffmpeg.exe";
		logMetod = " > ";
	}
	if ( _linux ){
		ffmpeg = "ffmpeg";
		logMetod = " &> ";
	}
	name=os::basename(folder);
	dirMovie=os::dirname(folder);
	list=dirMovie+"/list_"+name;

	//listado de movies en la carpeta
	movie_list="";
	auto dir_list = os::listdir(folder);
	sort( dir_list.begin(), dir_list.end() );
	for (auto i : dir_list){
		string ext = split( i, "." ).back(); 		
		if ( ext == "mov" )		
			movie_list+="file '"+i+"'\n";
	}
	//-------------------------

	//crea lista de archivos para el concat		
	fwrite(list, movie_list);
	//----------------------------------

	// crea ruta del exportacion
	movie=dirMovie+"/"+name+".mov";
	//-----------------------------------------

	if ( _win32 ){ safe = "-safe 0"; }
	else{ safe = ""; }

	concat = ffmpeg+" -y -f concat "+safe+" -i "+'"'+list+'"'+" -c copy "+'"'+movie+'"';
	null=dirMovie+"/null";
	cmd = concat+logMetod+'"'+null+'"';
	os::sh(cmd);

	//------------------------------------------
	os::remove(null);
	//-----------------------------------------------------

	// borra lista creada
	os::remove(list);
	//----------------------

	// borra carpeta de los videos separados
	if ( os::isfile(movie) )	
		os::remove(folder);
	//---------------------------------------

}

