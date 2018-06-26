#include "server.h"

string server::render_task( json recv ){
	debug("server::render_task: start render.");
	// comvierte lista de json en variables usables
	string status;

	int instance, first_frame, last_frame;
	string project, software, jobSystem, extra, render, vmSoftware;

	project = recv[0];
	software = recv[1];
	instance = recv[2];
	first_frame = recv[3];
	last_frame = recv[4];
	jobSystem = recv[5];
	extra = recv[6];
	render = recv[7];

	//------------------------------------------------------------

	// si alguna de las instancias ya esta en render no renderea
	bool renderNow = false;

	if ( not renderInstance[ instance ] ){
		renderInstance[ instance ] = true;
		renderNow = true;
		}
	//-----------------------------------------------------------------

	if ( renderNow ){

		json system_path = preferences["paths"]["system"];

		//obtiene ruta correcta
		string src_path, dst_path, proj;

		for ( string p1 : system_path ){
			for ( string p2 : system_path){
				proj = replace( project, p1, p2 );

				if ( os::isfile( proj ) ){
					src_path = p1;
					dst_path = p2;
					break;
				}

			}

			if ( os::isfile( proj ) ){
				break;
			}
		}

		//--------------------------------------

		string log_file = path() + "/log/render_log_" + to_string( instance );
		//os::remove( log_file );

		string cmd = pre_software(software, project, first_frame, last_frame, extra, 
									instance, src_path, dst_path, jobSystem, render, log_file);

		debug("server::render_task: renter to cmd.");
		// comienza a renderear, y rescata el pid para poder matar el proceso
		QProcess proc;
		proc.start( QString::fromStdString(cmd) );
		pid[instance] = proc.processId();
		proc.waitForFinished();
		QString output = proc.readAllStandardOutput();
		proc.close();

		if ( ( software == "Houdini" ) or ( software == "Nuke" ) ) 
			fwrite( log_file, output.toStdString() );

		//-------------------------------------

		if ( taskKill[instance] ){
			taskKill[instance] = false;
			status = "kill";
		}
		if ( not ( status == "kill" )){
			int total_frame = last_frame - first_frame + 1;
			status = post_software( total_frame, log_file, software );

			if ( status == "failed" ){
				os::copy( log_file, "../../log/render_log" );
			}
		}
		// Habilita las instancia que ya termino, para que pueda renderear		
		renderInstance[instance] = false;
		//---------------------------------------------------------------------

	}

	return status;
}

string server::pre_software(string software, string project, int first_frame, int last_frame,
	string extra, int instance, string src_path, string dst_path, 
	string jobSystem, string render, string log_file ){

	string cmd, called;

	json paths = preferences["paths"];

	if ( software == "Nuke" ){
 		debug("server::pre_software: Nuke.");
 		// crea la carpeta donde se renderearan los archivos
		string dirFile = replace( extra, src_path, dst_path );

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

		string args = "-f -X " + render + " \"" + project + "\" " + to_string( first_frame ) + "-" + to_string( last_frame);

		// remapeo rutas de Nuke
		string nukeRemap = " -remap \"" + src_path + "," + dst_path + "\" ";
		args = replace( args, src_path, dst_path );
		args = nukeRemap + args;
		//------------------------------------------------

		//Obtiene el excecutable que existe en este sistema
		string exe;
		for ( auto e : paths["nuke"]){
			 exe = e; //e.rstrip()
			 if ( os::isfile( exe ) ){ break; }
 		}
 		//-----------------------------------------------

		cmd = '"' + exe + '"' + args;
	}

	if ( software == "Houdini"){
		debug("server::pre_software: Houdini.");
		//Obtiene el excecutable que existe en este sistema
		string exe;
		for ( auto e : paths["houdini"] ){
			 exe = e; //e.rstrip()
			 if ( os::isfile( exe ) ){ break; }
 		}
 		//-----------------------------------------------

		string hipFile = replace( project, src_path, dst_path );

		string render_file = path() + "/modules/houdiniCatsFarm.py " + 
							hipFile + " " + render + " " + to_string( first_frame ) + " " + to_string( last_frame );

		cmd = '"' + exe + "\" " + render_file;

	}

	if ( software == "Maya" ){
		debug("server::pre_software: Maya.");
		string args = " -r file -s " + to_string( first_frame ) + " -e " + to_string( last_frame ) + 
					" -proj \"" + extra + "\" \"" + project + '"' + " -log \"" + log_file + "\"";

		//Obtiene el excecutable que existe en este sistema
		string exe;

		for ( auto e : paths["maya"] ){
			 exe = e;
			 if ( os::isfile( exe ) ){ break; }
 		}
 		//-----------------------------------------------
		args = replace( args, src_path, dst_path );

		//evita que maya de error al iniciar el sercio de carsfarm server al iniar la maquina
		string MAYA_DISABLE_CIP;
		if ( _linux ) MAYA_DISABLE_CIP = "export MAYA_DISABLE_CIP=1 && ";
		else MAYA_DISABLE_CIP = "";
		//--------------------------------------------------------------------------------------

		cmd = MAYA_DISABLE_CIP + '"' + exe + "\" " + args;

	}

	if ( software == "Fusion" ){
		debug("server::pre_software: Fusion.");
		string project = replace( project, src_path, dst_path );
		string extra = replace( extra, src_path, dst_path);

		string ext = extra.substr( extra.length() - 3 );
		if ( ext == "mov" ){

			string folder = extra.erase( extra.length() - 4 );
			string base_name = os::basename( folder );
			if ( not os::isdir( folder ) ){
				os::mkdir( folder );
				if (_linux ){
					system( ("chmod 777 -R " + folder).c_str() );
				}
			}
			string padding = ( "000000" + to_string( first_frame ) );
			padding = padding.erase( padding.length() - 7 );
			string output = folder + "/" + base_name + "_" + padding + "." + ext;
		}
		else{ string output = extra; }

		string args = " " + project + " -render -verbose " +
						" " + to_string( first_frame ) + ' ' + to_string( last_frame ) + " -quit ";

		//Obtiene el excecutable que existe en este sistema
		string exe;
		for ( auto e : paths["fusion"] ){
			 exe = e; //e.rstrip()
			 if ( os::isfile( exe ) ){ break; }
 		}
 		//-----------------------------------------------

		cmd = '"' + exe + '"' + args;

	}

	if ( software == "Cinema4D" ){
		debug("server::pre_software: Cinema4D.");
		project = replace( project, src_path, dst_path );

		string args = " -nogui -render \"" + project + "\" g_logfile=\"" + log_file + "\"" +
					 " -frame " + to_string( first_frame ) + " " + to_string( last_frame );

		//Obtiene el excecutable que existe en este sistema
		string exe;
		for ( auto e : paths["cinema"] ){
			 exe = e; //e.rstrip()
			 if ( os::isfile( exe ) ){ break; }
 		}
 		//-----------------------------------------------

		cmd = '"' + exe + '"' + args;
	}

	if ( software == "Noice" ){
		project = replace( project, src_path, dst_path );

		//Obtiene el excecutable que existe en este sistema
		string exe;
		for ( auto e : paths["noice"] ){
			 exe = e; //e.rstrip()
			 if ( os::isfile( exe ) ){ break; }
		}
		//-----------------------------------------------

		string pr = split( split( extra, "-pr ")[1], " -sr " )[0];
		string sr = split( split( extra, "-sr ")[1], " -v " )[0];
		string vr = split( split( extra, "-v ")[1], " -tr " )[0];
		string tr = split( extra, "-tr ")[1];

		string denoiser;
		if ( _linux ) denoiser = path() + "/bin/linux/denoiser";
		else if ( _win32 ) denoiser = path() + "/bin/win/denoiser.exe";
		else denoiser = path() + "/bin/mac/denoiser";

		cmd = denoiser + " \"" + project + "\" \"" + exe + "\" " + pr + " " + sr + " " + 
						vr + " " + tr + " " + to_string(first_frame) + " " + to_string(last_frame);
	
		print(cmd);

	}

	awrite( "../../log/called_log", cmd );
	return cmd;
}

string server::post_software( int total_frame, string log_file, string software){
	debug("server::post_software: start.");

	// Lee el log render
	string log = fread( log_file );
	//----------------------------------------

	string status;

	if ( software == "Nuke" ){

		auto frame_split = split( log, "Frame " );
		if ( int( frame_split.size() - 1 ) == total_frame ){ status = "ok"; }
		else{ status = "failed"; }
	}

	if ( software == "Houdini" ){
		string _frame = " frame ";

		int _frames = 0;
		int pos = log.find(_frame);
		while (pos != -1){
		    pos = log.find(_frame, pos + 7 );
			_frames++;
		}

		if ( _frames == total_frame ) status = "ok";
		else status = "failed";

	}

	if ( software == "Maya" ){

		if ( in_string( "completed.", log ) ){ status = "ok";}
		else{ status = "failed"; }
	}

	if ( software == "Cinema4D" ){
		if ( in_string( "Rendering successful: ", log ) ){ status = "ok";}
		else{ status = "failed"; }
	}

	if ( software=="Fusion" ){
		if ( in_string( "Render completed successfully", log ) ){ status = "ok";}
		else{ status = "failed"; }		
	}

	if ( software == "Noice" ) status = "ok";

	return status;
}
