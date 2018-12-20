#include <iostream>

using namespace std;

#include "util.h"
#include "os.h"
void concat(QString folder)
{

	QString ffmpeg, logMetod, dirMovie, list, concat, movie, movie_list, name, cmd, null;

	if (_win32)
	{
		ffmpeg = path + "/os/win/ffmpeg/ffmpeg.exe";
		logMetod = " > ";
	}
	if (_linux)
	{
		ffmpeg = "ffmpeg";
		logMetod = " &> ";
	}
	name = os::basename(folder);
	dirMovie = os::dirname(folder);
	list = dirMovie + "/list_" + name;

	//listado de movies en la carpeta
	movie_list = "";
	QStringList dir_list = os::listdir(folder);
	sort(dir_list.begin(), dir_list.end());
	for (auto i : dir_list)
	{
		QString ext = i.split(".").last();
		if (ext == "mov")
			movie_list += "file '" + i + "'\n";
	}
	//-------------------------

	//crea lista de archivos para el concat
	fwrite(list, movie_list);
	//----------------------------------

	// crea ruta del exportacion
	movie = dirMovie + "/" + name + ".mov";
	//-----------------------------------------

	if (_win32)
		list.replace("/", "\\");

	concat = ffmpeg + " -y -f concat -safe 0 -i " + '"' + list + '"' + " -c copy " + '"' + movie + '"';
	os::sh(concat);

	// borra lista creada
	os::remove(list);
	//----------------------

	// borra carpeta de los videos separados
	if (os::isfile(movie))
		os::remove(folder);
	//---------------------------------------
}
