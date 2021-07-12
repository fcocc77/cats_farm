#include "render.h"
#include "../global/global.h"
#include <hardware_monitor.h>
#include "video.h"

render_class::render_class(QMutex *_mutex)
    : mutex(_mutex)
{
    // inicializar instancias 16 veces
    for (int i = 0; i < 15; ++i)
    {
        instance_data data;

        data.first_frame = 0;
        data.last_frame = 0;
        data.pid = 0;
        data.task_kill = false;
        data.render_instance = false;
        data.software_data = {};
        data.job_system = "none";
        data.software = "";

        idata.push_back(data);
    }
}

QString render_class::render_task(QJsonObject _task)
{
    // comvierte lista de json en variables usables
    QString status;

    mutex->lock();
    int ins = _task["instance"].toInt();
    QString software = _task["software"].toString();

    idata[ins].software_data = _task["software_data"].toObject();
    idata[ins].first_frame = _task["first_frame"].toInt();
    idata[ins].last_frame = _task["last_frame"].toInt();
    idata[ins].job_system = _task["job_system"].toString();
    idata[ins].software = software;

    // si alguna de las instancias ya esta en render no renderea
    bool renderNow = false;

    if (not idata[ins].render_instance)
    {
        idata[ins].render_instance = true;
        renderNow = true;
    }

    mutex->unlock();
    if (renderNow)
    {
        bool renderOK = false;
        if (software == "nuke")
            renderOK = nuke(ins);
        if (software == "houdini")
            renderOK = houdini(ins);
        if (software == "maya")
            renderOK = maya(ins);
        if (software == "ffmpeg")
            renderOK = ffmpeg(ins);
        if (software == "vinacomp")
            renderOK = vinacomp(ins);

        QString log_file =
            VINARENDER_PATH + "/log/render_log_" + QString::number(ins);

        mutex->lock();
        if (idata[ins].task_kill)
        {
            idata[ins].task_kill = false;
            status = "kill";
        }
        else
        {
            if (renderOK)
                status = "ok";
            else
            {
                status = "failed";
                os::copy(log_file, VINARENDER_PATH + "/log/render_log");
            }
        }

        idata[ins].pid = 0;

        // Habilita la instancia que ya termino, para que pueda renderizar
        idata[ins].render_instance = false;
        mutex->unlock();
    }

    return status;
}

void render_class::get_correct_path(QString filename, QString *src, QString *dst)
{
    QMutexLocker locker(mutex);

    QJsonArray system_path =
        settings_from_manager["paths"].toObject()["system"].toArray();

    QString src_path, dst_path, aux;
    for (QJsonValue p1 : system_path)
    {
        for (QJsonValue p2 : system_path)
        {
            aux = filename;
            aux.replace(p1.toString(), p2.toString());

            if (os::isfile(aux))
            {
                src_path = p1.toString();
                dst_path = p2.toString();
                break;
            }
        }

        if (os::isfile(aux))
            break;
    }

    *src = src_path;
    *dst = dst_path;
}

QString render_class::qprocess(QString cmd, int ins, int timeout)
{
    QProcess proc;
    proc.start(cmd);
    if (ins > -1)
        idata[ins].pid = proc.processId();

    if (timeout != -1)
        timeout *= 1000;

    proc.waitForFinished(timeout);
    QString output =
        proc.readAllStandardOutput() + "\n" + proc.readAllStandardError();
    proc.close();

    return output;
}

QString render_class::get_executable(QString software) const
{
    for (QJsonValue value : settings_from_manager["paths"].toObject()[software].toArray())
        if (os::isfile(value.toString()))
            return value.toString();

    return "";
}

void render_class::log_save(int ins, QString cmd, QString log)
{
    QString log_file = VINARENDER_PATH + "/log/" + idata[ins].software + '_' +
                       QString::number(ins);

    QString _log;

    _log += "/////////////////////////////////////////////////\n\n";

    _log += os::hostName() + " - " + idata[ins].software + " - Instance Nº " +
            QString::number(ins) + ":\n\n\n";

    _log += cmd + "\n\n";
    _log += log;

    fwrite(log_file, _log);
}

bool render_class::nuke(int ins)
{
    mutex->lock();
    QJsonObject data = idata[ins].software_data;

    QString src_path, dst_path;

    QString filename = data["filename"].toString();
    QString render_node = data["render_node"].toString();
    QString proj = data["project"].toString();

    mutex->unlock();

    get_correct_path(proj, &src_path, &dst_path);

    filename.replace(src_path, dst_path);
    proj.replace(src_path, dst_path);

    QString temp_project = proj;
    temp_project.replace(".nk", "_" + os::hostName() + ".nk");

    QString nk = fread(proj);
    nk.replace(src_path, dst_path);
    fwrite(temp_project, nk);

    QString dirname = os::dirname(filename);
    QString basename = os::basename(filename);
    QString basename_no_ext = basename.split('.')[0];

    QString ext = basename.split(".").last();

    if (ext == "mov")
    {
        QString movie_chunks_dir = dirname + '/' + basename_no_ext;

        if (not os::isdir(movie_chunks_dir))
        {
            os::makedirs(movie_chunks_dir);
            if (_linux)
                os::system("chmod 777 -R " + movie_chunks_dir);
        }
    }

    // Si es hay licencia de nodo de render nuke_r poner true
    bool nuke_r = false;
    QString xi = nuke_r ? "" : "-xi";

    mutex->lock();

    QString exe = get_executable("nuke");
    QString cmd = "\"%1\" -remap \"%2,%3\" -f %4 -X %5 \"%6\" %7-%8";

    cmd = cmd.arg(exe, src_path, dst_path, xi, render_node, temp_project,
                  QString::number(idata[ins].first_frame),
                  QString::number(idata[ins].last_frame));

    cmd = cmd.replace(src_path, dst_path);

    mutex->unlock();

    // rendering ...
    QString log;
    log = qprocess(cmd, ins);

    log_save(ins, cmd, log);

    // Borra proyecto temporal
    os::remove(temp_project);

    mutex->lock();
    int total_frame = idata[ins].last_frame - idata[ins].first_frame + 1;
    mutex->unlock();

    if (log.count("Frame ") == total_frame)
        return true;
    else
        return false;
}

bool render_class::maya(int ins)
{
    QJsonObject data = idata[ins].software_data;

    QString scene = data["scene"].toString();
    QString project_folder = data["project_folder"].toString();

    QString src_path, dst_path;

    get_correct_path(scene, &src_path, &dst_path);

    QString log_file =
        VINARENDER_PATH + "/log/render_log_" + QString::number(ins);

    os::remove(log_file);

    QString cmd = "/bin/sh -c \"export MAYA_DISABLE_CIP=1 && \"%1\" -r file -s "
                  "%2 -e %3 -proj \"%4\" \"%5\" -log \"%6\"";

    cmd = cmd.arg(get_executable("maya"), QString::number(idata[ins].first_frame),
                  QString::number(idata[ins].last_frame), project_folder, scene,
                  log_file);

    cmd = cmd.replace(src_path, dst_path);

    qprocess(cmd, ins);

    // post render
    QString log = fread(log_file);
    if (log.contains("completed."))
        return true;
    else
        return false;
}

bool render_class::houdini(int ins)
{
    mutex->lock();
    QJsonObject data = idata[ins].software_data;

    QString src_path, dst_path;

    QString node_path = data["node_path"].toString();
    QString project = data["project"].toString();

    mutex->unlock();

    get_correct_path(project, &src_path, &dst_path);

    QString houdini_module = VINARENDER_PATH + "/modules/houdini_save_geo.py";

    QString exe = get_executable("houdini");
    QString cmd = "\"%1\" \"%2\" \"%3\" \"%4\" %5 %6 %7";

    cmd = cmd.arg(exe, houdini_module, project, node_path,
                  QString::number(idata[ins].first_frame),
                  QString::number(idata[ins].last_frame));

    cmd = cmd.replace(src_path, dst_path);

    qprocess(cmd, ins);

    return true;
}

bool render_class::ffmpeg(int ins)
{
    QJsonObject data = idata[ins].software_data;

    QString input_file = data["input_file"].toString();
    QString output_folder = data["output_folder"].toString();
    QString movie_name = data["movie_name"].toString();
    QString command = data["command"].toString();
    QString exe = get_executable("ffmpeg");

    video::meta meta = video::get_meta_data(input_file);

    QString src_path, dst_path;
    get_correct_path(input_file, &src_path, &dst_path);

    output_folder.replace(src_path, dst_path);

    float start_time =
        video::frame_to_seconds(idata[ins].first_frame, meta.frame_rate);

    float end_time = video::frame_to_seconds(idata[ins].last_frame, meta.frame_rate);
    end_time += (1.0 / meta.frame_rate) / 1.5;

    QString dir_file = output_folder + "/" + movie_name;

    if (!os::isdir(dir_file))
    {
        os::makedir(dir_file);
        os::system("chmod 777 -R \"" + dir_file + "\"");
    }

    QString number = "0000000000" + QString::number(idata[ins].first_frame);
    number = number.right(10);

    QString out_movie =
        QString("%1/%2_%3.%4").arg(dir_file, movie_name, number, "mov");

    QString cmd = "\"%1\" -y -i \"%2\" -ss %3 -to %4 %5 \"%6\"";

    cmd = cmd.arg(exe, input_file, QString::number(start_time),
                  QString::number(end_time), command, out_movie);

    qprocess(cmd);

    return true;
}

bool render_class::vinacomp(int ins)
{
    return false;
}
