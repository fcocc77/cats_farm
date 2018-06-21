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

	dirMovie=os::dirname(folder);
	list=dirMovie+"/list_"+os::basename(folder);

	//listado de movies en la carpeta
	movie_list="";
	auto dir_list = os::listdir(folder);
	sort( dir_list.begin(), dir_list.end() );
	for (auto i : dir_list){
		movie_list+="file '"+i+"'\n";
	}
	//-------------------------

	//crea lista de archivos para el concat		
	fwrite(list, movie_list);
	//----------------------------------

	// crea ruta del exportacion
	name=os::basename(dir_list[0]);
	name=name.substr( 0, (name.find_last_of("_") ) );
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
	os::remove(folder);
	//---------------------------------------

}

