#include "render.h"

QString render::render_task(QJsonArray recv)
{

	// comvierte lista de json en variables usables
	QString status;

	int ins = recv[2].toInt();
	QString software = recv[1].toString();
	project[ins] = recv[0].toString();
	first_frame[ins] = recv[3].toInt();
	last_frame[ins] = recv[4].toInt();
	jobSystem[ins] = recv[5].toString();
	extra[ins] = recv[6].toString();
	renderNode[ins] = recv[7].toString();
	//------------------------------------------------------------

	// si alguna de las instancias ya esta en render no renderea
	bool renderNow = false;

	if (not renderInstance[ins])
	{
		renderInstance[ins] = true;
		renderNow = true;
	}
	//-----------------------------------------------------------------

	if (renderNow)
	{
		QJsonArray system_path = preferences["paths"].toObject()["system"].toArray();

		//obtiene ruta correcta
		QString proj;
		for (QJsonValue p1 : system_path)
		{
			for (QJsonValue p2 : system_path)
			{
				proj = project[ins];
				proj.replace(p1.toString(), p2.toString());

				if (os::isfile(proj))
				{
					src_path[ins] = p1.toString();
					dst_path[ins] = p2.toString();
					break;
				}
			}

			if (os::isfile(proj))
			{
				break;
			}
		}

		// ---------- rendering softwares -----------------
		bool renderOK = false;
		if (software == "Nuke")
			renderOK = nuke(ins);
		if (software == "Houdini")
			renderOK = houdini(ins);
		if (software == "Maya")
			renderOK = maya(ins);
		if (software == "Fusion")
			renderOK = fusion(ins);
		if (software == "Cinema4D")
			renderOK = cinema(ins);
		if (software == "Natron")
			renderOK = natron(ins);
		// -------------------------------------------------

		QString log_file = path + "/log/render_log_" + QString::number(ins);

		if (taskKill[ins])
		{
			taskKill[ins] = false;
			status = "kill";
		}
		else
		{
			if (renderOK)
				status = "ok";
			else
			{
				status = "failed";
				os::copy(log_file, "../../log/render_log");
			}
		}

		// Habilita las instancia que ya termino, para que pueda renderear
		renderInstance[ins] = false;
		//---------------------------------------------------------------------
	}

	return status;
}

QString render::qprocess(QString cmd, int ins)
{
	QProcess proc;
	proc.start(cmd);
	if (ins > -1)
		pid[ins] = proc.processId();
	proc.waitForFinished(-1);
	QString output = proc.readAllStandardOutput() + "\n" + proc.readAllStandardError();
	proc.close();

	return output;
}

void render::vbox_turn(bool turn)
{

	QString vm;
	if (turn)
	{
		if (_linux)
			vm = "VBoxManage startvm win2016 --type headless";
		if (_win32)
			vm = "\"C:/Program Files/Oracle/VirtualBox/VBoxManage.exe\" startvm win2016 --type headless";

		os::back(vm);
	}
	else
	{
		if (_linux)
			vm = "VBoxManage controlvm win2016 savestate";
		if (_win32)
			vm = "\"C:/Program Files/Oracle/VirtualBox/VBoxManage.exe\" controlvm win2016 savestate";

		os::back(vm);

		fwrite(path + "/log/vbox", "0");
	}
}

bool render::vbox_working()
{
	// Virtual Machin status

	QString vm;
	if (_linux)
		vm = "VBoxManage list runningvms";

	if (_win32)
		vm = "\"C:/Program Files/Oracle/VirtualBox/VBoxManage.exe\" list runningvms";

	QString running = os::sh(vm).split(" ")[0];

	if (running == "\"win2016\"")
	{
		return true;
	}
	else
	{
		return false;
	}
	//------------------------------------------
}

void render::suspend_vbox()
{

	// checkea si el Cinama 4D esta en render y si no se apaga la maquina virtual
	VMCinemaRunningTimes = 0;
	while (1)
	{
		if (not VMCinemaActive)
		{
			VMCinemaRunningTimes++;
			if (VMCinemaRunningTimes > 10)
			{ // si no se esta usando Cinema4D, al numero 10 se apaga la maquina
				if (vbox_working() and VMCinemaTurn)
				{ // solo si esta prendida y si la prendio catsfarm
					vbox_turn(false);
					VMCinemaTurn = false;
				}
				VMCinemaRunningTimes = 0;
			}
		}
		sleep(1);
	} //-----------------------------------------------------------------------
}

bool render::nuke(int ins)
{

	// crea la carpeta donde se renderearan los archivos
	QString dirFile = extra[ins].replace(src_path[ins], dst_path[ins]);

	QString dirRender = os::dirname(dirFile);
	QString fileRender = os::basename(dirFile);

	QString ext = fileRender.split(".").last();
	fileRender = fileRender.split(".")[0];
	QString pad = fileRender.split("_").last();

	QString folder;
	if (ext == "mov")
	{
		folder = fileRender;
	}
	else
	{
		folder = fileRender.replace("_" + pad, "");
	}

	QString folderRender = dirRender + "/" + folder;

	if (not os::isdir(folderRender))
	{
		os::mkdir(folderRender);
		if (_linux)
			os::system("chmod 777 -R " + folderRender);
	}
	//---------------------------------------------------

	QString args = "-f -X " + renderNode[ins] + " \"" + project[ins] + "\" " + QString::number(first_frame[ins]) + "-" + QString::number(last_frame[ins]);

	// remapeo rutas de Nuke
	QString nukeRemap = " -remap \"" + src_path[ins] + "," + dst_path[ins] + "\" ";
	args = args.replace(src_path[ins], dst_path[ins]);
	args = nukeRemap + args;
	//------------------------------------------------

	//Obtiene el excecutable que existe en este sistema
	QString exe;
	for (auto e : preferences["paths"].toObject()["nuke"].toArray())
	{
		exe = e.toString();
		if (os::isfile(exe))
		{
			break;
		}
	}
	//-----------------------------------------------

	QString cmd = '"' + exe + '"' + args;

	QString log_file = path + "/log/render_log_" + QString::number(ins);

	// rendering ...
	// ----------------------------------
	QString log;
	log = qprocess(cmd, ins);
	// al hacer render en una comp nueva por primera vez
	// aparece un error de "missing close-brace" y para evitar que
	// llegue el error al manager intenta en un segundo si vuelve
	// a aparecer manda el error
	if (log.contains("missing close-brace"))
	{
		sleep(1);
		log = qprocess(cmd, ins);
	}
	// ---------------------------------------
	fwrite(log_file, log);
	// ----------------------------------

	int total_frame = last_frame[ins] - first_frame[ins] + 1;

	if (log.count("Frame ") == total_frame)
		return true;
	else
		return false;
}

bool render::maya(int ins)
{

	QString log_file = path + "/log/render_log_" + QString::number(ins);
	os::remove(log_file);

	QString args = " -r file -s " + QString::number(first_frame[ins]) + " -e " + QString::number(last_frame[ins]) +
				   " -proj \"" + extra[ins] + "\" \"" + project[ins] + '"' + " -log \"" + log_file + "\"";

	//Obtiene el excecutable que existe en este sistema
	QString exe;

	for (auto e : preferences["paths"].toObject()["maya"].toArray())
	{
		exe = e.toString();
		if (os::isfile(exe))
		{
			break;
		}
	}
	//-----------------------------------------------
	args = args.replace(src_path[ins], dst_path[ins]);

	QString cmd = '"' + exe + "\" " + args;
	// rendering ...
	// ----------------------------------
	qprocess(cmd, ins);
	// ----------------------------------

	// post render
	QString log = fread(log_file);
	if (log.contains("completed."))
		return true;
	else
		return false;
	// --------------------------
}

bool render::cinema(int ins)
{

	QString log_file = path + "/log/render_log_" + QString::number(ins);
	QString log, cmd;

	if (_linux)
	{ // en linux se usa una maquina virtual
		VMCinemaActive = true;
		//Obtiene el excecutable de cinema de windows
		QString exe_windows;
		for (auto e : preferences["paths"].toObject()["cinema"].toArray())
		{
			exe_windows = e.toString();
			if (exe_windows.contains("C:/"))
				break;
		}
		//-----------------------------------------------

		// replaza las rutas para virtualbox
		for (auto s : preferences["paths"].toObject()["system"].toArray())
			project[ins] = project[ins].replace(s.toString(), "//VBOXSVR/server_01");
		//-------------------------------------------------------------------------

		// con este commando se puede enviar comandos a la maquina virtual y te regresa un resultado
		QString guestcontrol = "VBoxManage --nologo guestcontrol win2016 run --username Administrator --password Jump77cats --exe ";
		VMSH = guestcontrol + "C:\\\\Windows\\\\system32\\\\cmd.exe -- C:\\\\Windows\\\\SysWOW64\\\\cmd.exe \"/c\" ";
		// --------------------------------------------------------------

		QString args = "\"-frame\" \"" + QString::number(first_frame[ins]) + "\" \"" + QString::number(last_frame[ins]) + "\" \"-nogui\" \"-render\" \"" + project[ins] + "\"";
		cmd = guestcontrol + "\"" + exe_windows + "\" -- 0 " + args; // 0 es el primer argumento que seria el excecutable que esta en -exe

		// inicia virtual machine
		if (not vbox_working())
		{
			vbox_turn(true);
			VMCinemaTurn = true;
		}

		// checkea si la maquina esta lista para renderear
		QString check = VMSH + "\"echo vm_is_ready\"";

		int i = 0;
		bool problem = false;
		while (not qprocess(check).contains("vm_is_ready"))
		{ // espera que la maquina este lista
			sleep(1);
			if (i > 200)
			{
				problem = true;
				log = "The Virtual Machine has a problem.";
				break;
			}
		}
		//--------------------------------

		// rendering ...
		// ----------------------------------
		if (not problem)
			log = qprocess(cmd, ins);
		//-------------------------------
		fwrite(log_file, log);

		//para que se apague la maquina virtual si no se uas
		VMCinemaActive = false;
		VMCinemaRunningTimes = 0;
		//----------------------------
	}

	else
	{

		project[ins] = project[ins].replace(src_path[ins], dst_path[ins]);

		QString args = " -nogui -render \"" + project[ins] + "\" g_logfile=\"" + log_file + "\"" +
					   " -frame " + QString::number(first_frame[ins]) + " " + QString::number(last_frame[ins]);

		//Obtiene el excecutable que existe en este sistema
		QString exe;
		for (auto e : preferences["paths"].toObject()["cinema"].toObject())
		{
			exe = e.toString();
			if (os::isfile(exe))
			{
				break;
			}
		}
		//-----------------------------------------------

		cmd = '"' + exe + '"' + args;

		// rendering ...
		// ----------------------------------
		qprocess(cmd, ins);
		log = fread(log_file);
		//----------------------
	}

	// post render
	if (log.contains("Rendering successful: "))
		return true;
	else
		return false;
	//---------------------------
}

bool render::houdini(int ins)
{

	//Obtiene el excecutable que existe en este sistema
	QString exe;
	for (auto e : preferences["paths"].toObject()["houdini"].toArray())
	{
		exe = e.toString();
		if (os::isfile(exe))
		{
			break;
		}
	}
	//-----------------------------------------------

	QString hipFile = project[ins].replace(src_path[ins], dst_path[ins]);

	QString render_file = path + "/modules/houdiniCatsFarm.py " +
						  hipFile + " " + renderNode[ins] + " " + QString::number(first_frame[ins]) + " " + QString::number(last_frame[ins]);

	QString cmd = '"' + exe + "\" " + render_file;

	QString log_file = path + "/log/render_log_" + QString::number(ins);

	// rendering ...
	// ----------------------------------
	QString log = qprocess(cmd, ins);
	fwrite(log_file, log);
	// ----------------------------------

	// post render
	int total_frame = last_frame[ins] - first_frame[ins] + 1;

	if (log.count(" frame ") == total_frame)
		return true;
	else
		return false;
	// ----------------------------------
}

bool render::fusion(int ins)
{

	QString log_file = path + "/log/render_log_" + QString::number(ins);
	os::remove(log_file);

	project[ins] = project[ins].replace(src_path[ins], dst_path[ins]);

	if (_win32)
	{
		project[ins] = project[ins].replace("/", "\\");
		log_file = log_file.replace("/", "\\");
	}

	// replmaza rutas internas del .comp
	QString inside_project = fread(project[ins]);
	if (_linux)
		inside_project = inside_project.replace("\\\\", "/");
	inside_project = inside_project.replace(src_path[ins], dst_path[ins]);
	QString new_project = path + "/etc/fusion_render_" + QString::number(ins) + ".comp";
	fwrite(new_project, inside_project);
	//------------------------------------------------------------------------

	QString args = " " + new_project + " --verbose --render --start " + QString::number(first_frame[ins]) + " --end " + QString::number(last_frame[ins]) + " -log " + log_file;

	//Obtiene el excecutable que existe en este sistema
	QString exe;
	for (auto e : preferences["paths"].toObject()["fusion"].toArray())
	{
		exe = e.toString();
		if (os::isfile(exe))
		{
			break;
		}
	}
	//-----------------------------------------------

	QString cmd;
	if (_win32)
		cmd = '"' + exe + '"' + args;
	if (_linux)
	{
		os::sh("chmod 777 -R " + path + "/log");						 // al renderear en user no tiene los permisos de root en el log asi le da permisos
		cmd = "runuser -l " + os::user() + " -c \"" + exe + args + "\""; // se ejecuta en usuario por que nececita X11 DISPLAY
	}
	// rendering ...
	// ----------------------------------
	qprocess(cmd, ins);
	// ----------------------------------

	// post render
	QString log = fread(log_file);
	if (log.contains("Render completed successfully"))
		return true;
	else
		return false;
	//-----------------------------------------------
}

bool render::natron(int ins)
{

	QString args = "-i " + renderNode[ins] + " " + QString::number(first_frame[ins]) + "-" + QString::number(last_frame[ins]) + " \"" + project[ins] + "\"";

	args = args.replace(src_path[ins], dst_path[ins]);

	//Obtiene el excecutable que existe en este sistema
	QString exe;
	for (auto e : preferences["paths"].toObject()["natron"].toArray())
	{
		exe = e.toString();
		if (os::isfile(exe))
		{
			break;
		}
	}
	//-----------------------------------------------

	QString cmd = '"' + exe + '"' + args;

	QString log_file = path + "/log/render_log_" + QString::number(ins);

	// rendering ...
	// ----------------------------------
	QString log = qprocess(cmd, ins);
	fwrite(log_file, log);
	// ----------------------------------

	return false;
}
