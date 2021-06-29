#include "render.h"
#include "../global/global.h"
#include <hardware_monitor.h>

render_class::render_class(QMutex *_mutex)
    : mutex(_mutex)
{
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
    }
}

QString render_class::render_task(QJsonArray recv)
{
    // comvierte lista de json en variables usables
    QString status;

    mutex->lock();
    int ins = recv[2].toInt();
    QString software = recv[1].toString().toLower();
    project[ins] = recv[0].toString();
    first_frame[ins] = recv[3].toInt();
    last_frame[ins] = recv[4].toInt();
    jobSystem[ins] = recv[5].toString();
    extra[ins] = recv[6].toString();
    renderNode[ins] = recv[7].toString();

    // si alguna de las instancias ya esta en render no renderea
    bool renderNow = false;

    if (not renderInstance[ins])
    {
        renderInstance[ins] = true;
        renderNow = true;
    }

    mutex->unlock();
    if (renderNow)
    {
        mutex->lock();

        auto correctPath = find_correct_path(project[ins]);
        src_path[ins] = correctPath[0];
        dst_path[ins] = correctPath[1];

        mutex->unlock();

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
                os::copy(log_file, VINARENDER_PATH + "/log/render_log");
            }
        }

        pid[ins] = 0;

        // Habilita la instancia que ya termino, para que pueda renderizar
        renderInstance[ins] = false;
        mutex->unlock();
    }

    return status;
}

QList<QString> render_class::find_correct_path(QString file_path)
{
    QString _path = os::dirname(os::dirname(file_path));
    QJsonArray system_path =
        preferences["paths"].toObject()["system"].toArray();

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
    QString output =
        proc.readAllStandardOutput() + "\n" + proc.readAllStandardError();
    proc.close();

    return output;
}

bool render_class::nuke(int ins)
{
    mutex->lock();

    auto correctPath = find_correct_path(extra[ins]);

    QString proj = project[ins];
    proj.replace(src_path[ins], dst_path[ins]);

    QString tmpProj = proj;
    tmpProj.replace(".nk", "_" + os::hostName() + ".nk");

    QString nk = fread(proj);
    nk.replace(correctPath[0], correctPath[1]);

    fwrite(tmpProj, nk);

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
        os::makedirs(folderRender);
        if (_linux)
            os::system("chmod 777 -R " + folderRender);
    }

    // Si es hay licencia de nodo de render nuke_r poner true
    bool nuke_r = false;
    QString xi = "";
    if (not nuke_r)
        xi = "-xi";

    mutex->lock();
    QString args = "-f " + xi + " -X " + renderNode[ins] + " \"" + tmpProj +
                   "\" " + QString::number(first_frame[ins]) + "-" +
                   QString::number(last_frame[ins]);

    // remapeo rutas de Nuke
    QString nukeRemap =
        " -remap \"" + src_path[ins] + "," + dst_path[ins] + "\" ";
    args = args.replace(src_path[ins], dst_path[ins]);
    args = nukeRemap + args;

    // Obtiene el excecutable que existe en este sistema
    QString exe;
    for (auto e : preferences["paths"].toObject()["nuke"].toArray())
    {
        exe = e.toString();
        if (os::isfile(exe))
            break;
    }
    mutex->unlock();

    QString cmd = '"' + exe + '"' + args;

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
    QString log_file =
        VINARENDER_PATH + "/log/render_log_" + QString::number(ins);
    os::remove(log_file);

    QString args = " -r file -s " + QString::number(first_frame[ins]) + " -e " +
                   QString::number(last_frame[ins]) + " -proj '" + extra[ins] +
                   "' '" + project[ins] + "'" + " -log '" + log_file + "'";

    // Obtiene el excecutable que existe en este sistema
    QString exe;

    for (auto e : preferences["paths"].toObject()["maya"].toArray())
    {
        exe = e.toString();
        if (os::isfile(exe))
        {
            break;
        }
    }
    args = args.replace(src_path[ins], dst_path[ins]);

    QString cmd = "/bin/sh -c \"export MAYA_DISABLE_CIP=1 && '" + exe + "' " +
                  args + "\"";
    // rendering ...
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

    // Obtiene el excecutable que existe en este sistema
    QString exe;
    for (auto e : preferences["paths"].toObject()["houdini"].toArray())
    {
        exe = e.toString();
        if (os::isfile(exe))
        {
            break;
        }
    }

    QString hipFile = project[ins].replace(src_path[ins], dst_path[ins]);

    QString render_file = VINARENDER_PATH + "/modules/houdiniVinaRender.py " +
                          hipFile + " " + renderNode[ins] + " " +
                          QString::number(first_frame[ins]) + " " +
                          QString::number(last_frame[ins]);

    QString cmd = '"' + exe + "\" " + render_file;

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
    return false;
}

bool render_class::vinacomp(int ins)
{
    return false;
}
