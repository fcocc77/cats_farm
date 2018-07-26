#include "render.h"

string render::render_task( json recv ){
	debug("server::render_task: start render.");
	// comvierte lista de json en variables usables

	string status;

	int ins = recv[2];
	string software = recv[1];

	project[ ins ] = recv[0];

	first_frame[ ins ] = recv[3];
	last_frame[ ins ] = recv[4];
	jobSystem[ ins ] = recv[5];
	extra[ ins ] = recv[6];
	renderNode[ ins ] = recv[7];

	//------------------------------------------------------------

	// si alguna de las instancias ya esta en render no renderea
	bool renderNow = false;

	if ( not renderInstance[ ins ] ){
		renderInstance[ ins ] = true;
		renderNow = true;
		}
	//-----------------------------------------------------------------

	if ( renderNow ){

		json system_path = preferences["paths"]["system"];

		//obtiene ruta correcta
		string proj;

		for ( string p1 : system_path ){
			for ( string p2 : system_path){
				proj = replace( project[ ins ], p1, p2 );

				if ( os::isfile( proj ) ){
					src_path[ ins ] = p1;
					dst_path[ ins ] = p2;
					break;
				}

			}

			if ( os::isfile( proj ) ){
				break;
			}
		}

		// ---------- rendering softwares -----------------
		bool renderOK = false;
		if ( software == "Nuke" ) renderOK = nuke( ins );
		if ( software == "Houdini" ) renderOK = houdini( ins );
		if ( software == "Maya" ) renderOK = maya( ins );
		if ( software == "Fusion" ) renderOK = fusion( ins );
		if ( software == "Cinema4D" ) renderOK = cinema( ins );
		if ( software == "Noice" ) renderOK = noice( ins );		
		if ( software == "Ffmpeg" ) renderOK = ffmpeg( ins );
		// -------------------------------------------------

		string log_file = path() + "/log/render_log_" + to_string( ins );

		if ( taskKill[ins] ){
			taskKill[ins] = false;
			status = "kill";
		}
		else {
			if ( renderOK )
				status = "ok";
			else{
				status = "failed";
				os::copy( log_file, "../../log/render_log" );
			}
		}

		// Habilita las instancia que ya termino, para que pueda renderear		
		renderInstance[ins] = false;
		//---------------------------------------------------------------------

	}

	return status;
}

string render::qprocess( string cmd, int ins ){
	QProcess proc;
	proc.start( QString::fromStdString(cmd) );
	pid[ins] = proc.processId();
	proc.waitForFinished(-1);
	QString output = proc.readAllStandardOutput();
	proc.close();

	return output.toStdString();
}

bool render::nuke( int ins ){

	// crea la carpeta donde se renderearan los archivos
	string dirFile = replace( extra[ ins ], src_path[ ins ], dst_path[ ins ] );

	string dirRender = os::dirname( dirFile );
	string fileRender = os::basename( dirFile );

	string ext = fileRender.substr( fileRender.length() - 3 );
	fileRender = fileRender.erase( fileRender.length() - 4 );
	string pad = split( fileRender, "_" ).back();

	string folder;
	if ( ext == "mov" ){ folder = fileRender; }
	else{ folder = replace( fileRender, "_" + pad, "" ); }

	string folderRender = dirRender + "/" + folder;

	if ( not os::isdir( folderRender ) ){
		os::mkdir( folderRender );
		if ( _linux ) os::system( "chmod 777 -R " + folderRender );
	}
	//---------------------------------------------------

	string args = "-f -X " + renderNode[ ins ] + " \"" + project[ ins ] + "\" " + to_string( first_frame[ ins ] ) + "-" + to_string( last_frame[ ins ] );

	// remapeo rutas de Nuke
	string nukeRemap = " -remap \"" + src_path[ ins ] + "," + dst_path[ ins ] + "\" ";
	args = replace( args, src_path[ ins ], dst_path[ ins ] );
	args = nukeRemap + args;
	//------------------------------------------------

	//Obtiene el excecutable que existe en este sistema
	string exe;
	for ( auto e : preferences["paths"]["nuke"]){
		 exe = e; //e.rstrip()
		 if ( os::isfile( exe ) ){ break; }
	}
	//-----------------------------------------------

	string cmd = '"' + exe + '"' + args;

	string log_file = path() + "/log/render_log_" + to_string( ins );
	os::remove( log_file );

	// rendering ...
	// ----------------------------------
	string log = qprocess( cmd, ins );
	fwrite( log_file, log );
	// ----------------------------------

	auto frame_split = split( log, "Frame " );
	int total_frame = last_frame[ ins ] - first_frame[ ins ] + 1;
	if ( int( frame_split.size() - 1 ) == total_frame ) return true;
	else return false;
}

bool render::maya( int ins ){

	string log_file = path() + "/log/render_log_" + to_string( ins );
	os::remove( log_file );

	string args = " -r file -s " + to_string( first_frame[ ins ] ) + " -e " + to_string( last_frame[ ins ] ) + 
				" -proj \"" + extra[ ins ] + "\" \"" + project[ ins ] + '"' + " -log \"" + log_file + "\"";

	//Obtiene el excecutable que existe en este sistema
	string exe;

	for ( auto e : preferences["paths"]["maya"] ){
		 exe = e;
		 if ( os::isfile( exe ) ){ break; }
		}
		//-----------------------------------------------
	args = replace( args, src_path[ ins ], dst_path[ ins ] );

	//evita que maya de error al iniciar el servicio de catsfarm server al iniciar la maquina
	string MAYA_DISABLE_CIP;
	if ( _linux ) MAYA_DISABLE_CIP = "export MAYA_DISABLE_CIP=1 && ";
	else MAYA_DISABLE_CIP = "";
	//--------------------------------------------------------------------------------------

	string cmd = MAYA_DISABLE_CIP + '"' + exe + "\" " + args;

	// rendering ...
	// ----------------------------------
	qprocess( cmd, ins );
	// ----------------------------------

	// post render 
	string log = fread( log_file );
	if ( in_string( "completed.", log ) ) return true;
	else return false;
	// --------------------------
}

bool render::houdini( int ins ){

	//Obtiene el excecutable que existe en este sistema
	string exe;
	for ( auto e : preferences["paths"]["houdini"] ){
		 exe = e;
		 if ( os::isfile( exe ) ){ break; }
		}
		//-----------------------------------------------

	string hipFile = replace( project[ ins ], src_path[ ins ], dst_path[ ins ] );

	string render_file = path() + "/modules/houdiniCatsFarm.py " + 
						hipFile + " " + renderNode[ ins ] + " " + to_string( first_frame[ ins ] ) + " " + to_string( last_frame[ ins ] );

	string cmd = '"' + exe + "\" " + render_file;

	string log_file = path() + "/log/render_log_" + to_string( ins );
	os::remove( log_file );

	// rendering ...
	// ----------------------------------
	string log = qprocess( cmd, ins );
	fwrite( log_file, log );
	// ----------------------------------

	// post render
	string _frame = " frame ";

	int _frames = 0;
	int pos = log.find(_frame);
	while (pos != -1){
	    pos = log.find(_frame, pos + 7 );
		_frames++;
	}

	int total_frame = last_frame[ ins ] - first_frame[ ins ] + 1;
	if ( _frames == total_frame ) return true;
	else return false;
	// ----------------------------------
}

bool render::cinema( int ins ){

	string log_file = path() + "/log/render_log_" + to_string( ins );
	os::remove( log_file );

	project[ ins ] = replace( project[ ins ], src_path[ ins ], dst_path[ ins ] );

	string args = " -nogui -render \"" + project[ ins ] + "\" g_logfile=\"" + log_file + "\"" +
				 " -frame " + to_string( first_frame[ ins ] ) + " " + to_string( last_frame[ ins ] );

	//Obtiene el excecutable que existe en este sistema
	string exe;
	for ( auto e : preferences["paths"]["cinema"] ){
		 exe = e; 
		 if ( os::isfile( exe ) ){ break; }
		}
		//-----------------------------------------------

	string cmd = '"' + exe + '"' + args;

	// rendering ...
	// ----------------------------------
	qprocess( cmd, ins );
	// ----------------------------------

	// post render
	string log = fread( log_file );
	if ( in_string( "Rendering successful: ", log ) ) return true;
	else return false;
}

bool render::fusion( int ins ){

	string log_file = path() + "/log/render_log_" + to_string( ins );
	os::remove( log_file );

	project[ ins ] = replace( project[ ins ], src_path[ ins ], dst_path[ ins ] );
	extra[ ins ] = replace( extra[ ins ], src_path[ ins ], dst_path[ ins ] );

	string ext = extra[ ins ].substr( extra[ ins ].length() - 3 );
	if ( ext == "mov" ){

		string folder = extra[ ins ].erase( extra[ ins ].length() - 4 );
		string base_name = os::basename( folder );
		if ( not os::isdir( folder ) ){
			os::mkdir( folder );
			if (_linux ){
				system( ("chmod 777 -R " + folder).c_str() );
			}
		}
		string padding = ( "000000" + to_string( first_frame[ ins ] ) );
		padding = padding.erase( padding.length() - 7 );
		string output = folder + "/" + base_name + "_" + padding + "." + ext;
	}
	else{ string output = extra[ ins ]; }

	string args = " " + project[ ins ] + " -render -verbose " +
					" " + to_string( first_frame[ ins ] ) + ' ' + to_string( last_frame[ ins ] ) + " -quit ";

	//Obtiene el excecutable que existe en este sistema
	string exe;
	for ( auto e : preferences["paths"]["fusion"] ){
		 exe = e;
		 if ( os::isfile( exe ) ){ break; }
		}
		//-----------------------------------------------

	string cmd = '"' + exe + '"' + args;

	// rendering ...
	// ----------------------------------
	qprocess( cmd, ins );
	// ----------------------------------

	// post render
	string log = fread( log_file );
	if ( in_string( "Render completed successfully", log ) ) return true;
	else return false;
	//-----------------------------------------------
}

bool render::noice( int ins ){
	project[ ins ] = replace( project[ ins ], src_path[ ins ], dst_path[ ins ] );

	//Obtiene el excecutable que existe en este sistema
	string exe;
	for ( auto e : preferences["paths"]["noice"] ){
		 exe = e;
		 if ( os::isfile( exe ) ){ break; }
	}
	//-----------------------------------------------

	string pr = split( split( extra[ ins ], "-pr ")[1], " -sr " )[0];
	string sr = split( split( extra[ ins ], "-sr ")[1], " -v " )[0];
	string vr = split( split( extra[ ins ], "-v ")[1], " -tr " )[0];
	string tr = split( extra[ ins ], "-tr ")[1];

	string denoiser;
	if ( _linux ) denoiser = path() + "/bin/linux/denoiser";
	else if ( _win32 ) denoiser = path() + "/bin/win/denoiser.exe";
	else denoiser = path() + "/bin/mac/denoiser";

	string cmd = denoiser + " \"" + project[ ins ] + "\" \"" + exe + "\" " + pr + " " + sr + " " + 
					vr + " " + tr + " " + to_string( first_frame[ ins ] ) + " " + to_string( last_frame[ ins ] );

	return true;
}

bool render::ffmpeg( int ins ){
	return false;
}