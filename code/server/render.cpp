#include "render.h"

QString render::render_task( QJsonObject recv ){

	// comvierte lista de json en variables usables
	QString status;

	int ins = recv[2].toInt();
	QString software = recv[1].toString();

	project[ ins ] = recv[0].toInt();

	first_frame[ ins ] = recv[3].toInt();
	last_frame[ ins ] = recv[4].toInt();
	jobSystem[ ins ] = recv[5].toString();
	extra[ ins ] = recv[6].toString();
	renderNode[ ins ] = recv[7].toString();
	//------------------------------------------------------------

	// si alguna de las instancias ya esta en render no renderea
	bool renderNow = false;

	if ( not renderInstance[ ins ] ){
		renderInstance[ ins ] = true;
		renderNow = true;
		}
	//-----------------------------------------------------------------

	if ( renderNow ){

		QJsonObject system_path = preferences["paths"].toObject()["system"].toObject();

		//obtiene ruta correcta
		QString proj;

		for ( QString p1 : system_path ){
			for ( QString p2 : system_path){
				proj = replace( project[ ins ], p1.toString(), p2.toString() );

				if ( os::isfile( proj ) ){
					src_path[ ins ] = p1.toString();
					dst_path[ ins ] = p2.toString();
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

		QString log_file = path() + "/log/render_log_" + QString::number( ins );

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

QString render::qprocess( QString cmd, int ins ){
	QProcess proc;
	proc.start( QString::fromStdString(cmd) );
	if ( ins > -1 ) pid[ins] = proc.processId();
	proc.waitForFinished(-1);
	QString output = proc.readAllStandardOutput() + "\n" + proc.readAllStandardError();
	proc.close();

	return output.toStdString();
}

bool render::nuke( int ins ){

	// crea la carpeta donde se renderearan los archivos
	QString dirFile = replace( extra[ ins ], src_path[ ins ], dst_path[ ins ] );

	QString dirRender = os::dirname( dirFile );
	QString fileRender = os::basename( dirFile );

	QString ext = fileRender.substr( fileRender.length() - 3 );
	fileRender = fileRender.erase( fileRender.length() - 4 );
	QString pad = split( fileRender, "_" ).back();

	QString folder;
	if ( ext == "mov" ){ folder = fileRender; }
	else{ folder = replace( fileRender, "_" + pad, "" ); }

	QString folderRender = dirRender + "/" + folder;

	if ( not os::isdir( folderRender ) ){
		os::mkdir( folderRender );
		if ( _linux ) os::system( "chmod 777 -R " + folderRender );
	}
	//---------------------------------------------------

	QString args = "-f -X " + renderNode[ ins ] + " \"" + project[ ins ] + "\" " + to_string( first_frame[ ins ] ) + "-" + to_string( last_frame[ ins ] );

	// remapeo rutas de Nuke
	QString nukeRemap = " -remap \"" + src_path[ ins ] + "," + dst_path[ ins ] + "\" ";
	args = replace( args, src_path[ ins ], dst_path[ ins ] );
	args = nukeRemap + args;
	//------------------------------------------------

	//Obtiene el excecutable que existe en este sistema
	QString exe;
	for ( auto e : preferences["paths"]["nuke"]){
		 exe = e; //e.rstrip()
		 if ( os::isfile( exe ) ){ break; }
	}
	//-----------------------------------------------

	QString cmd = '"' + exe + '"' + args;

	QString log_file = path() + "/log/render_log_" + to_string( ins );

	// rendering ...
	// ----------------------------------
	QString log = qprocess( cmd, ins );
	fwrite( log_file, log );
	// ----------------------------------

	auto frame_split = split( log, "Frame " );
	int total_frame = last_frame[ ins ] - first_frame[ ins ] + 1;
	if ( int( frame_split.size() - 1 ) == total_frame ) return true;
	else return false;
}

bool render::maya( int ins ){

	QString log_file = path() + "/log/render_log_" + to_string( ins );
	os::remove( log_file );

	QString args = " -r file -s " + to_string( first_frame[ ins ] ) + " -e " + to_string( last_frame[ ins ] ) + 
				" -proj \"" + extra[ ins ] + "\" \"" + project[ ins ] + '"' + " -log \"" + log_file + "\"";

	//Obtiene el excecutable que existe en este sistema
	QString exe;

	for ( auto e : preferences["paths"]["maya"] ){
		 exe = e;
		 if ( os::isfile( exe ) ){ break; }
		}
		//-----------------------------------------------
	args = replace( args, src_path[ ins ], dst_path[ ins ] );

	QString cmd = '"' + exe + "\" " + args;

	// rendering ...
	// ----------------------------------
	qprocess( cmd, ins );
	// ----------------------------------

	// post render 
	QString log = fread( log_file );
	if ( in_string( "completed.", log ) ) return true;
	else return false;
	// --------------------------
}

bool render::houdini( int ins ){

	//Obtiene el excecutable que existe en este sistema
	QString exe;
	for ( auto e : preferences["paths"]["houdini"] ){
		 exe = e;
		 if ( os::isfile( exe ) ){ break; }
		}
		//-----------------------------------------------

	QString hipFile = replace( project[ ins ], src_path[ ins ], dst_path[ ins ] );

	QString render_file = path() + "/modules/houdiniCatsFarm.py " + 
						hipFile + " " + renderNode[ ins ] + " " + to_string( first_frame[ ins ] ) + " " + to_string( last_frame[ ins ] );

	QString cmd = '"' + exe + "\" " + render_file;

	QString log_file = path() + "/log/render_log_" + to_string( ins );

	// rendering ...
	// ----------------------------------
	QString log = qprocess( cmd, ins );
	fwrite( log_file, log );
	// ----------------------------------

	// post render
	QString _frame = " frame ";

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

void render::vbox_turn( bool turn ){

	QString vm;
	if ( turn ){
		if ( _linux ) vm = "VBoxManage startvm win2016 --type headless";
		if ( _win32 ) vm = "\"C:/Program Files/Oracle/VirtualBox/VBoxManage.exe\" startvm win2016 --type headless";

		os::back( vm );
	}
	else{
		if ( _linux ) vm = "VBoxManage controlvm win2016 savestate";
		if ( _win32 ) vm = "\"C:/Program Files/Oracle/VirtualBox/VBoxManage.exe\" controlvm win2016 savestate";

		os::back( vm );

		fwrite( path() + "/log/vbox", "0" );
	}
}

bool render::vbox_working(){
	// Virtual Machin status

	QString vm;
	if ( _linux ) vm = "VBoxManage list runningvms";

	if ( _win32 ) vm = "\"C:/Program Files/Oracle/VirtualBox/VBoxManage.exe\" list runningvms";

	QString running = split( os::sh(vm), " " )[0];

	if ( running == "\"win2016\"" ){ return true; }
	else { return false; }
	//------------------------------------------
}

void render::suspend_vbox(){

	// checkea si el Cinama 4D esta en render y si no se apaga la maquina virtual
	VMCinemaRunningTimes = 0;
	while (1){
		if ( not VMCinemaActive ) {
			VMCinemaRunningTimes ++;
			if ( VMCinemaRunningTimes > 10 ) { // si no se esta usando Cinema4D, al numero 10 se apaga la maquina 
				if ( vbox_working() and VMCinemaTurn ) { // solo si esta prendida y si la prendio catsfarm
					vbox_turn( false ); 
					VMCinemaTurn = false;
				}
				VMCinemaRunningTimes = 0;
			}
		}
		sleep(1);
	} //-----------------------------------------------------------------------
}

bool render::cinema( int ins ){

	QString log_file = path() + "/log/render_log_" + to_string( ins );
	QString log, cmd;

	if ( _linux ){ // en linux se usa una maquina virtual
		VMCinemaActive = true;
		//Obtiene el excecutable de cinema de windows
		QString exe_windows;
		for ( auto e : preferences["paths"]["cinema"] ){
			exe_windows = e; 
			if ( in_string( "C:/", exe_windows ) ) break;
		}
		//-----------------------------------------------

		// replaza las rutas para virtualbox
		for ( auto s :  preferences["paths"]["system"] )
			project[ ins ] = replace( project[ ins ], s, "//VBOXSVR/server_01" );
		//-------------------------------------------------------------------------

		// con este commando se puede enviar comandos a la maquina virtual y te regresa un resultado
		QString guestcontrol = "VBoxManage --nologo guestcontrol win2016 run --username Administrator --password Jump77cats --exe ";
		VMSH = guestcontrol + "C:\\\\Windows\\\\system32\\\\cmd.exe -- C:\\\\Windows\\\\SysWOW64\\\\cmd.exe \"/c\" ";
		// --------------------------------------------------------------

		QString args = "\"-frame\" \"" + to_string( first_frame[ ins ] ) + "\" \"" +  to_string( last_frame[ ins ] ) + "\" \"-nogui\" \"-render\" \"" + project[ ins ] + "\"";
		cmd = guestcontrol + "\"" + exe_windows + "\" -- 0 " + args; // 0 es el primer argumento que seria el excecutable que esta en -exe

		// inicia virtual machine
		if ( not vbox_working() ) {
			vbox_turn( true );
			VMCinemaTurn = true;
		}

		// checkea si la maquina esta lista para renderear
		QString check = VMSH + "\"echo vm_is_ready\"";

		int i = 0;		
		bool problem = false;
		while( not in_string( "vm_is_ready", qprocess( check ) ) ){ // espera que la maquina este lista
			sleep(1);
			if ( i > 200 ) {
				problem = true;
				log = "The Virtual Machine has a problem.";
				break;
			}
		}
		//--------------------------------

		// rendering ...
		// ----------------------------------
		if ( not problem ) log = qprocess( cmd, ins );
		//-------------------------------
		fwrite( log_file, log );

		//para que se apague la maquina virtual si no se uas
		VMCinemaActive = false;
		VMCinemaRunningTimes = 0;
		//----------------------------
	}

	else{

		project[ ins ] = replace( project[ ins ], src_path[ ins ], dst_path[ ins ] );

		QString args = " -nogui -render \"" + project[ ins ] + "\" g_logfile=\"" + log_file + "\"" +
					 " -frame " + to_string( first_frame[ ins ] ) + " " + to_string( last_frame[ ins ] );

		//Obtiene el excecutable que existe en este sistema
		QString exe;
		for ( auto e : preferences["paths"]["cinema"] ){
			 exe = e; 
			 if ( os::isfile( exe ) ){ break; }
			}
			//-----------------------------------------------

		cmd = '"' + exe + '"' + args;

		// rendering ...
		// ----------------------------------
		qprocess( cmd, ins );
		log = fread( log_file );
		//----------------------
	}

	// post render
	if ( in_string( "Rendering successful: ", log ) ) return true;
	else return false;
	//---------------------------
}

bool render::fusion( int ins ){

	QString log_file = path() + "/log/render_log_" + to_string( ins );
	os::remove( log_file );

	project[ ins ] = replace( project[ ins ], src_path[ ins ], dst_path[ ins ] );

	if ( _win32 ){
		project[ ins ] = replace( project[ ins ], "/", "\\" );
		log_file = replace( log_file, "/", "\\" );
	}

	// replmaza rutas internas del .comp
	QString inside_project = fread( project[ ins ] );
	if ( _linux ) inside_project = replace( inside_project, "\\\\", "/");
	inside_project = replace( inside_project, src_path[ ins ], dst_path[ ins ] );
	QString new_project = path() + "/etc/fusion_render_" + to_string( ins ) + ".comp";
	fwrite( new_project, inside_project );
	//------------------------------------------------------------------------

	QString args = " " + new_project + " --verbose --render --start " + to_string( first_frame[ ins ] ) + " --end " + to_string( last_frame[ ins ] ) + " -log " + log_file;

	//Obtiene el excecutable que existe en este sistema
	QString exe;
	for ( auto e : preferences["paths"]["fusion"] ){
		 exe = e;
		 if ( os::isfile( exe ) ){ break; }
	}
	//-----------------------------------------------

	QString cmd;
	if ( _win32 ) cmd = '"' + exe + '"' + args;
	if ( _linux ) {
		os::sh( "chmod 777 -R " + path() + "/log" ); // al renderear en user no tiene los permisos de root en el log asi le da permisos
		cmd = "runuser -l " + os::user() + " -c \"" + exe + args + "\""; // se ejecuta en usuario por que nececita X11 DISPLAY
	}
	// rendering ...
	// ----------------------------------
	qprocess( cmd, ins );
	// ----------------------------------

	// post render
	QString log = fread( log_file );
	if ( in_string( "Render completed successfully", log ) ) return true;
	else return false;
	//-----------------------------------------------
}

bool render::noice( int ins ){
	project[ ins ] = replace( project[ ins ], src_path[ ins ], dst_path[ ins ] );
	QString input = project[ ins ];

	//Obtiene el excecutable que existe en este sistema
	QString exe;
	for ( auto e : preferences["paths"]["noice"] ){
		 exe = e;
		 if ( os::isfile( exe ) ){ break; }
	}
	//-----------------------------------------------

	// conversion de extra a parametros de noice
	QString pr = split( split( extra[ ins ], "-pr ")[1], " -sr " )[0];
	QString sr = split( split( extra[ ins ], "-sr ")[1], " -v " )[0];
	QString vr = split( split( extra[ ins ], "-v ")[1], " -tr " )[0];
	int tr = stoi( split( split( extra[ ins ], "-tr ")[1], " -aov " )[0] );
	QString aov = split( extra[ ins ], "-aov ")[1];
	//------------------------------------------------

	// get aov
	QString layers;
	if ( not aov.empty() )
		for ( QString l : split( aov, "," ) ) 
			layers += " -l " + l;
	//-------------------------------------

	QString log_file = path() + "/log/render_log_" + to_string( ins );
	QString log;
	int frames_ok = 0; 
	
	for ( int frame = first_frame[ ins ]; frame <= last_frame[ ins ]; ++frame ){

		auto getFrame = []( QString _path, int frame_ ){

			QString dirname = os::dirname(_path);
			QString basename = os::basename(_path);

			QString ext = split( basename, "." ).back();
			QString pad_ext = split( basename, "_" ).back();
			QString basename_without = replace( basename, pad_ext, "");

			QString cero = "";
			QString _frame;
			for (int i = 0; i < 10; ++i){

				_frame = dirname + "/" + basename_without + cero + to_string(frame_) + "." + ext;
				cero += "0";

				if ( os::isfile(_frame) ) return _frame;

			}

			return _frame;
		};

		int _fr = tr;
		vector <QString> inputs;

		// agrega los frames anteriores
		int _frame = frame ;
		for (int i = 0; i < tr; ++i){
			_frame = _frame -1;

			QString frame_path = getFrame( input, _frame );
			if ( frame_path == "none" ) _fr += 1;
			else inputs.push_back( frame_path );
		}
		reverse( inputs.begin(), inputs.end() ); 
		//-----------------------------------------

		// agrega los frames posteriores
		int _frame2 = frame ;
		for (int i = 0; i < _fr; ++i){
			_frame2 = _frame2 + 1;

			QString frame_path = getFrame( input, _frame2 );
			if ( not ( frame_path == "none" ) ) 
				inputs.push_back( frame_path );
		}
		//------------------------------------------

		QString inputs_list;
		for ( auto i : inputs ) inputs_list += " -i \"" + i + "\"";
		//---------------------------------------------------

		QString _first_frame = getFrame( input, frame );

		// tranform input to output
		QString dirname = os::dirname(_first_frame);
		QString basename = os::basename(_first_frame);
		QString ext = split( basename, "." ).back();
		QString pad_ext = split( basename, "_" ).back();
		QString basename_without = replace( basename, pad_ext, "" );
		//-------------------------
		QString output = dirname + "/" + basename_without + "noice_" + pad_ext;
		//-------------------------------------------------------
		QString parameter = layers + " -pr " + pr + " -sr " + sr + " -v " + vr;
		QString cmd = "\"" + exe + "\"" + parameter +" -i \"" + _first_frame + "\"" + inputs_list + " -o \"" + output + "\"";
		// rendering ...
		// ----------------------------------
		QString _log = qprocess( cmd, ins );
		// ----------------------------------

		if ( in_string( "Finished denoising", _log ) ) frames_ok++;
		else break;

		log += "Frame: " + to_string( frame ) + "\n" + _log;
	}

	fwrite( log_file, log );

	int total_frame = last_frame[ ins ] - first_frame[ ins ] + 1;	
	
	if ( frames_ok == total_frame ) return true;
	else return false;
}

bool render::ffmpeg( int ins ){
	return false;
}
