#include "render.hpp"

render_class::render_class(QMutex *_mutex)
{
	mutex = _mutex;
	// inicializar instancias 16 veces
	for (int i = 0; i < 15; ++i)
	{
		first_frame.push_back(0);
		last_frame.push_back(0);
		pid.push_back(0);

		taskKill.push_back(false);
		renderInstance.push_back(false);

		project.push_back("none");
		jobSystem.push_back("none");
		extra.push_back("none");
		renderNode.push_back("none");
		vmSoftware.push_back("none");
		src_path.push_back("none");
		dst_path.push_back("none");
	} //-------------------------------------------

	// evita que quede alguna instancia de natron corriendo
	// si es que se llega a caer vserver.
	os::sh("pkill -9 NatronRenderer");
	// --------------------------
}

QString render_class::render_task(QJsonArray recv)
{
	// comvierte lista de json en variables usables
	QString status;

	mutex->lock();
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
	mutex->unlock();
	if (renderNow)
	{
		mutex->lock();
		//obtiene ruta correcta
		QJsonArray system_path = preferences["paths"].toObject()["system"].toArray();

		auto correctPath = find_correct_path(system_path, project[ins]);
		src_path[ins] = correctPath[0];
		dst_path[ins] = correctPath[1];
		//------------------------------------------------------
		mutex->unlock();

		// ---------- rendering softwares -----------------
		bool renderOK = false;
		if (software == "Nuke")
			renderOK = nuke(ins);
		if (software == "Houdini")
			renderOK = houdini(ins);
		if (software == "Maya")
			renderOK = maya(ins);
		if (software == "Natron")
			renderOK = natron(ins);
		if (software == "Ntp")
			renderOK = ntp(ins);
		if (software == "AE")
			renderOK = ae(ins);
		// -------------------------------------------------

		QString log_file = path + "/log/render_log_" + QString::number(ins);

		mutex->lock();
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
				os::copy(log_file, path + "/log/render_log");
			}
		}

		// deja la pid en 0
		pid[ins] = 0;
		// --------------

		// Habilita las instancia que ya termino, para que pueda renderear
		renderInstance[ins] = false;
		//----------------------------------
		mutex->unlock();
	}

	return status;
}

QList<QString> render_class::find_correct_path(QJsonArray system_path, QString _path)
{
	//obtiene ruta correcta
	QString proj;
	QString src;
	QString dst;
	for (QJsonValue p1 : system_path)
	{
		for (QJsonValue p2 : system_path)
		{
			proj = _path;
			proj.replace(p1.toString(), p2.toString());

			if (os::isfile(proj) || os::isdir(proj))
			{
				src = p1.toString();
				dst = p2.toString();
				break;
			}
		}

		if (os::isfile(proj) || os::isdir(proj))
			break;
	}

	return {src, dst};
}

QString render_class::qprocess(QString cmd, int ins, int timeout)
{
	QProcess proc;
	proc.start(cmd);
	if (ins > -1)
		pid[ins] = proc.processId();

	if (timeout != -1)
		timeout *= 1000;

	proc.waitForFinished(timeout);
	QString output = proc.readAllStandardOutput() + "\n" + proc.readAllStandardError();
	proc.close();

	return output;
}

bool render_class::nuke(int ins)
{
	mutex->lock();
	// Ecuentra ruta del write y la remplaza por la ruta correcta segun OS
	QJsonArray system_path = preferences["paths"].toObject()["system"].toArray();
	auto correctPath = find_correct_path(system_path, os::dirname(extra[ins]));

	QString proj = project[ins];
	proj.replace(src_path[ins], dst_path[ins]);

	QString tmpProj = proj;
	tmpProj.replace(".nk", "_" + os::hostName() + ".nk");

	QString nk = fread(proj);
	nk.replace(correctPath[0], correctPath[1]);

	fwrite(tmpProj, nk);
	// ------------------------------------------------------

	// crea la carpeta donde se renderearan los archivos
	QString dirFile = extra[ins].replace(correctPath[0], correctPath[1]);
	mutex->unlock();

	QString dirRender = os::dirname(dirFile);
	QString fileRender = os::basename(dirFile);

	QString ext = fileRender.split(".").last();
	fileRender = fileRender.split(".")[0];
	QString pad = fileRender.split("_").last();

	QString folder_name;
	if (ext == "mov")
		folder_name = fileRender;
	else
		folder_name = fileRender.replace("_" + pad, "");

	QString folderRender = dirRender + "/" + folder_name;

	if (not os::isdir(folderRender))
	{
		os::mkdir(folderRender);
		if (_linux)
			os::system("chmod 777 -R " + folderRender);
	}
	//---------------------------------------------------
	// Si es hay licencia de nodo de render nuke_r poner true
	bool nuke_r = false;
	QString xi = "";
	if (not nuke_r)
		xi = "-xi";
	// ----------------------------------
	mutex->lock();
	QString args = "-f " + xi + " -X " + renderNode[ins] + " \"" + tmpProj + "\" " + QString::number(first_frame[ins]) + "-" + QString::number(last_frame[ins]);
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
			break;
	}
	mutex->unlock();
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

	// Borra proyecto temporal
	os::remove(tmpProj);
	// -----------------------------

	mutex->lock();
	int total_frame = last_frame[ins] - first_frame[ins] + 1;
	mutex->unlock();

	if (log.count("Frame ") == total_frame)
		return true;
	else
		return false;
}

bool render_class::maya(int ins)
{

	QString log_file = path + "/log/render_log_" + QString::number(ins);
	os::remove(log_file);

	QString args = " -r file -s " + QString::number(first_frame[ins]) + " -e " + QString::number(last_frame[ins]) +
				   " -proj '" + extra[ins] + "' '" + project[ins] + "'" + " -log '" + log_file + "'";

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

	QString cmd = "/bin/sh -c \"export MAYA_DISABLE_CIP=1 && '" + exe + "' " + args + "\"";
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

bool render_class::houdini(int ins)
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

	QString render_file = path + "/modules/houdiniVinaRender.py " +
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

void render_class::natron_monitoring(int ins)
{
	// Chequea si NatronRenderer se cuelga:
	// si NatronRenderer tiene la cpu bajo los 30 ya esta conjelado,
	// y cada 5 segundos checkea esto y si esta bajo mata el proceso.

	// espera que el pid del instance este lista
	int _pid = 0;
	while (!_pid)
	{
		if (QThread::currentThread()->isInterruptionRequested())
			return;

		mutex->lock();
		_pid = pid[ins];
		mutex->unlock();

		sleep(1);
	}

	// espera 1 segundo por si a caso, para que el "qprocess" cargue NatronRenderer
	sleep(1);
	// ------------------

	// obtiene la id del NatronRenderer
	QString child_out = os::sh("pgrep -P " + QString::number(_pid));
	int natron_renderer_pid = child_out.toInt();
	// ---------------------

	if (natron_renderer_pid == 0)
		return;

	int natron_active = 0;
	while (1)
	{
		if (QThread::currentThread()->isInterruptionRequested())
		{
			// si se congela el NatronRenderer despues que ya termino el render, mata el proceso
			// para asegurarnos que no quede el proceso activo y consumiendo RAM.
			os::kill(natron_renderer_pid, true);
			return;
		}

		int usage = os::processCpuUsed(natron_renderer_pid);

		// si la cpu esta bajo 30 suma 1 a la actividad de natron
		if (usage < 30)
			natron_active += 1;
		else
			natron_active = 0;
		// ------------------------

		// si NatronRenderer estubo 4 veces bajo del 30 en cpu,
		// significa que se colgo, y mata el proceso de NatronRenderer.
		if (natron_active >= 4)
			os::kill(natron_renderer_pid, true);
		// ---------------------------

		sleep(5);
	}
}

bool render_class::natron(int ins)
{
	mutex->lock();

	//Obtiene el excecutable que existe en este sistema
	QString exe;
	for (auto e : preferences["paths"].toObject()["natron"].toArray())
	{
		exe = e.toString();
		if (os::isfile(exe))
			break;
	}
	//-----------------------------------------------
	QJsonObject _extra = jofs(extra[ins]);

	QString firstFrame = QString::number(first_frame[ins]);
	QString lastFrame = QString::number(last_frame[ins]);

	QString natron_module = path + "/modules/natron/render.sh";

	// crea la carpeta donde se renderearan los archivos
	QString output_file = _extra["output"].toString();

	QString output_dir = os::dirname(output_file);
	QString output_name = os::basename(output_file);

	QString ext = output_name.split(".").last();

	QString output_render;
	QString output;
	if (ext == "mov")
	{
		output_name = output_name.split(".")[0];
		output_render = output_dir + "/" + output_name;

		// crea numero con ceros para el nombre a partir del primer cuadro
		QString num = "0000000000" + firstFrame;
		QStringRef nameNumber(&num, num.length() - 10, 10);
		// -------------------------------------

		output = output_render + "/" + output_name + "_" + nameNumber + ".mov";

		if (not os::isdir(output_render))
		{
			os::mkdir(output_render);
			if (_linux)
				os::system("chmod 777 -R " + output_render);
		}
		//---------------------------------------------------
	}
	else
	{
		if (not os::isdir(output_dir))
		{
			os::mkdir(output_dir);
			if (_linux)
				os::system("chmod 777 -R " + output_dir);
		}

		output = output_file;
	}

	QString cmd = "sh " + natron_module + " " + exe + " \"" + project[ins] + "\" " + renderNode[ins] + " \"" + output + "\" " + firstFrame + " " + lastFrame;

	mutex->unlock();

	// para poder renderizar, cuando hay muchos proyectos con diferentes frames,
	// se necesita identificar que proyectos contienen el rango de frames de la tarea a renderizar,
	// con esto se genera una lista con los proyectos necesarios con su ruta correspondiente.
	bool divided_project = _extra["divided_project"].toBool();
	QJsonArray ranges_to_render;
	if (divided_project)
	{
		QJsonArray divided_projects = _extra["divided_projects"].toArray();
		for (QJsonValue value : divided_projects)
		{
			QJsonObject proj = value.toObject();
			QString proj_path = proj["project"].toString();
			int _first_frame = proj["first_frame"].toInt();
			int _last_frame = proj["last_frame"].toInt();

			bool insert = false;

			if (_first_frame <= last_frame[ins] && _first_frame >= first_frame[ins])
				insert = true;
			if (last_frame[ins] <= _last_frame && last_frame[ins] >= _first_frame)
				insert = true;
			if (_last_frame <= last_frame[ins] && _last_frame >= first_frame[ins])
				insert = true;
			if (last_frame[ins] <= _last_frame && last_frame[ins] >= _first_frame)
				insert = true;

			if (insert)
				ranges_to_render.push_back(QJsonArray({proj_path, _first_frame, _last_frame}));
		}
	}
	else
	{
		ranges_to_render.push_back(QJsonArray({project[ins], first_frame[ins], last_frame[ins]}));
	}
	// ---------------------------------

	for (QJsonValue i : ranges_to_render)
	{
		QString project_path = i.toArray()[0].toString();
		int _fist_frame = i.toArray()[1].toInt();
		int _last_frame = i.toArray()[2].toInt();

		// clamp de first y last frame a partir del rango de la tarea
		if (_fist_frame < first_frame[ins])
			_fist_frame = first_frame[ins];
		if (_last_frame > last_frame[ins])
			_last_frame = last_frame[ins];
		// ---------------------
	}

	QThread *thread = new QThread;
	connect(thread, &QThread::started, [=]() {
		natron_monitoring(ins);
	});
	thread->start();

	QString log;

	// si NatronRenderer se congelo con la cpu al 100%, se soluciona,
	// dandole un timeout al render de 7 minutos, si esta pegado
	// a los 7 minutos mata el proceso.
	log = qprocess(cmd, ins, 420);
	// ------------------

	thread->quit();
	thread->requestInterruption();

	QString log_file = path + "/log/render_log_" + QString::number(ins);
	fwrite(log_file, log);

	// post render
	if (log.contains("Rendering finished"))
		if (log.contains("Render aborted"))
			return false;
		else
			return true;
	else
		return false;
	//-----------------------------------------------
}

bool render_class::ntp(int ins)
{
	mutex->lock();
	int slide_index = first_frame[ins];
	QJsonArray slides = jafs(extra[ins]);
	QJsonObject slide = slides[slide_index].toObject();

	slide["project"] = project[ins];
	slide["output_folder"] = renderNode[ins];

	//Obtiene el excecutable que existe en este sistema
	QString exe;
	for (auto e : preferences["paths"].toObject()["ntp"].toArray())
	{
		exe = e.toString();
		if (os::isfile(exe))
			break;
	}
	//-----------------------------------------------

	QString ntp_module = path + "/modules/natron/ntp.sh";
	QString cmd = "sh " + ntp_module + " \"" + exe + "\" \"" + jots(slide).replace("\"", "'") + "\"";

	mutex->unlock();

	QThread *thread = new QThread;
	connect(thread, &QThread::started, [=]() {
		natron_monitoring(ins);
	});
	thread->start();

	QString log = qprocess(cmd, ins);

	QString log_file = path + "/log/render_log_" + QString::number(ins);
	fwrite(log_file, log);

	thread->quit();
	thread->requestInterruption();

	// post render
	if (log.contains("Testing Error: 0"))
		return true;
	else
		return false;
	//---------------------------
}

bool render_class::ae(int ins)
{
	QString log_file = path + "/log/render_log_" + QString::number(ins);
	os::remove(log_file);

	QString folderRender = os::dirname(extra[ins]);
	// Crea carpeta de renders
	if (not os::isdir(folderRender))
	{
		os::mkdir(folderRender);
		if (_linux)
			os::system("chmod 777 -R " + folderRender);
	}
	//---------------------------------------------------

	QString firstFrame = QString::number(first_frame[ins]);
	QString lastFrame = QString::number(last_frame[ins]);

	// crea numero con ceros para el nombre a partir del primer cuadro
	QString num = "0000000000" + firstFrame;
	QStringRef nameNumber(&num, num.length() - 10, 10);
	// -------------------------------------
	QString output = extra[ins] + "_" + nameNumber + ".mov";
	QString tmp = extra[ins] + "_" + nameNumber + "_tmp.mov";
	os::remove(output); // borra el mov antes del render

	QString args = "\"" + renderNode[ins] + "\" \"" + project[ins] + "\" \"" + output + "\" \"" + log_file + "\" " + firstFrame + " " + lastFrame + " " + QString::number(ins);

	args = args.replace(src_path[ins], dst_path[ins]);

	QString cmd = "sh " + path + "/modules/ae/aerender.sh " + args;
	// rendering ...
	// ----------------------------------
	qprocess(cmd, ins);
	// ----------------------------------

	// crea una version mas liviana ".mp4" del video y luego borra el original ".mov"
	QString postRender = "ffmpeg -i \"" + output + "\" -b:v 5M -c:a pcm_s16le " + tmp;
	os::sh(postRender);
	os::remove(output);
	os::rename(tmp, output);
	// ----------------------------------------

	// post render
	QString log = fread(log_file);
	if (log.contains("Finished composition"))
		return true;
	else
		return false;
	//-----------------------------------------------
}
