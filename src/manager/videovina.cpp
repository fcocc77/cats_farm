#include "manager.h"

QString manager::videovina(QJsonArray recv)
{
    // Rutas generales
    QString slideshow = "/home/pancho/Documents/GitHub/videovina/private/slideshows";
    QString musicDir = "/home/pancho/Documents/GitHub/videovina/private/music";
    QString as3 = "/home/pancho/Documents/GitHub/videovina/static/amazon_s3";
    QString localFolder = "/home/pancho/Desktop/renders";
    // -------------------------------

    // Datos recividos del servidor principal
    QString user = recv[0].toString();
    QString projectType = recv[1].toString();
    QString projectName = recv[2].toString();
    // -------------------------------------

    QString vvProjectDir = as3 + "/" + user + "/projects/" + projectName;
    QString vvProjectJson = vvProjectDir + "/project.json";
    QJsonObject vvProject = jread(vvProjectJson);

    QString userDir = localFolder + "/" + user;
    QString projectDir = userDir + "/" + projectName;

    os::makedirs(userDir);

    // Copia la plantilla del proyecto en el directorio local
    os::copydir(slideshow + "/" + projectType, projectDir);
    // -------------------------------------------

    // Copia footage, musica y project.json de amazon S3 y lo copia en el directorio compartido local
    os::copydir(vvProjectDir + "/footage", projectDir);
    os::copy(vvProjectJson, projectDir + "/project.json");
    QString song = vvProject["song"].toString() + ".mp3";
    os::copy(musicDir + "/" + song, projectDir + "/music/" + song);
    // -------------------------------------------

    QString project = projectDir + "/" + projectType + ".aep";

    // Modifica el proyecto de after effect con los datos del proyecto del usuario
    QString vina2ae = "/home/pancho/Documents/GitHub/cats_farm/modules/videovina/vina2ae.jsx";
    QString afterfx = "/opt/AE9.0/AfterFX.exe";
    QString cmd = "wine " + afterfx + " -noui -s \"var aep = '" + project + "';//@include '" + vina2ae + "';\"";
    os::system(cmd);
    // ---------------------------------------------------

    // Lee los datos de salida del script vina2ae.jsx
    QString submitJson = projectDir + "/submit.json";
    QJsonObject submit = jread(submitJson);
    // ---------------------------------------------

    QString job_name = user + "-" + projectName;
    QString server = "None";
    QString server_group = "videovina";
    int first_frame = 1;
    int last_frame = submit["last_frame"].toInt();
    int task_size = 10;
    int priority = 1;
    bool suspend = false;
    QString comment = "VideoVina Render";
    QString software = "AE";
    QString extra = "";
    QString _system = "linux";
    int instances = 1;
    QString render = "Final comp";

    QJsonArray data = {
        job_name,
        server,
        server_group,
        first_frame,
        last_frame,
        task_size,
        priority,
        suspend,
        comment,
        software,
        project,
        extra,
        _system,
        instances,
        render

    };

    //make_job(data);

    return "";
}
