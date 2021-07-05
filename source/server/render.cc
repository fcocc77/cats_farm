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
        first_frame.push_back(0);
        last_frame.push_back(0);
        pid.push_back(0);

        task_kill.push_back(false);
        render_instance.push_back(false);

        software_data.push_back({});
        job_system.push_back("none");
    }
}

QString render_class::render_task(QJsonObject _task)
{
    // comvierte lista de json en variables usables
    QString status;

    mutex->lock();
    int ins = _task["instance"].toInt();
    QString software = _task["software"].toString();

    software_data[ins] = _task["software_data"].toObject();
    first_frame[ins] = _task["first_frame"].toInt();
    last_frame[ins] = _task["last_frame"].toInt();
    job_system[ins] = _task["job_system"].toString();

    // si alguna de las instancias ya esta en render no renderea
    bool renderNow = false;

    if (not render_instance[ins])
    {
        render_instance[ins] = true;
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
        if (task_kill[ins])
        {
            task_kill[ins] = false;
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

        pid[ins] = 0;

        // Habilita la instancia que ya termino, para que pueda renderizar
        render_instance[ins] = false;
        mutex->unlock();
    }

    return status;
}

void render_class::get_correct_path(QString filename, QString *src, QString *dst)
{
    QMutexLocker locker(mutex);

    QJsonArray system_path =
        preferences["paths"].toObject()["system"].toArray();

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
        pid[ins] = proc.processId();

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
    for (QJsonValue value : preferences["paths"].toObject()[software].toArray())
        if (os::isfile(value.toString()))
            return value.toString();

    return "";
}

bool render_class::nuke(int ins)
{
    mutex->lock();

    QJsonObject data = software_data[ins];
    print(data);
    QString src_path, dst_path;

    QString output_movie = data["output_movie"].toString();
    QString render_node = data["render_node"].toString();
    QString proj = data["project"].toString();

    get_correct_path(output_movie, &src_path, &dst_path);

    output_movie.replace(src_path, dst_path);
    proj.replace(src_path, dst_path);

    QString tmpProj = proj;
    tmpProj.replace(".nk", "_" + os::hostName() + ".nk");

    QString nk = fread(proj);
    nk.replace(src_path, dst_path);

    fwrite(tmpProj, nk);

    mutex->unlock();

    QString dirRender = os::dirname(output_movie);
    QString fileRender = os::basename(output_movie);

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
        os::makedirs(folderRender);
        if (_linux)
            os::system("chmod 777 -R " + folderRender);
    }

    // Si es hay licencia de nodo de render nuke_r poner true
    bool nuke_r = false;
    QString xi = nuke_r ? "" : "-xi";

    mutex->lock();

    QString exe = get_executable("nuke");
    QString cmd = "\"%1\" -remap \"%2,%3\" -f %4 -X %5 \"%6\" %7-%8";

    cmd = cmd.arg(exe, src_path, dst_path, xi, render_node, tmpProj,
                  QString::number(first_frame[ins]),
                  QString::number(last_frame[ins]));

    cmd = cmd.replace(src_path, dst_path);

    mutex->unlock();

    QString log_file =
        VINARENDER_PATH + "/log/render_log_" + QString::number(ins);

    // rendering ...
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
    fwrite(log_file, log);

    // Borra proyecto temporal
    os::remove(tmpProj);

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
    QJsonObject data = software_data[ins];

    QString scene = data["scene"].toString();
    QString project_folder = data["project_folder"].toString();

    QString src_path, dst_path;
    get_correct_path(scene, &src_path, &dst_path);

    QString log_file =
        VINARENDER_PATH + "/log/render_log_" + QString::number(ins);

    os::remove(log_file);

    QString cmd = "/bin/sh -c \"export MAYA_DISABLE_CIP=1 && \"%1\" -r file -s "
                  "%2 -e %3 -proj \"%4\" \"%5\" -log \"%6\"";

    cmd = cmd.arg(get_executable("maya"), QString::number(first_frame[ins]),
                  QString::number(last_frame[ins]), project_folder, scene,
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

    QJsonObject data = software_data[ins];
    QString hip_file = data["project"].toString();
    QString engine_node = data["engine"].toString();

    QString src_path, dst_path;
    get_correct_path(hip_file, &src_path, &dst_path);

    hip_file.replace(src_path, dst_path);

    QString cmd = "\"%1\" %2 %3 %4 %5 %6";

    cmd = cmd.arg(get_executable("houdini"),
                  VINARENDER_PATH + "/modules/houdiniVinaRender.py", hip_file,
                  engine_node, QString::number(first_frame[ins]),
                  QString::number(last_frame[ins]));

    QString log_file =
        VINARENDER_PATH + "/log/render_log_" + QString::number(ins);

    // rendering ...
    QString log = qprocess(cmd, ins);
    fwrite(log_file, log);

    // post render
    int total_frame = last_frame[ins] - first_frame[ins] + 1;

    if (log.count(" frame ") == total_frame)
        return true;
    else
        return false;
}

bool render_class::ffmpeg(int ins)
{
    QJsonObject data = software_data[ins];

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
        video::frame_to_seconds(first_frame[ins], meta.frame_rate);

    float end_time = video::frame_to_seconds(last_frame[ins], meta.frame_rate);
    end_time += (1.0 / meta.frame_rate) / 1.5;

    QString dir_file = output_folder + "/" + movie_name;

    if (!os::isdir(dir_file))
    {
        os::makedir(dir_file);
        os::system("chmod 777 -R \"" + dir_file + "\"");
    }

    QString number = "0000000000" + QString::number(first_frame[ins]);
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
