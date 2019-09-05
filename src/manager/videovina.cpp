#include "manager.h"

QString manager::videovina(QJsonArray recv)
{
    // Rutas generales
    QString assets = "/home/pancho/Documents/GitHub/videovina/private";
    QString as3 = "/home/pancho/Documents/GitHub/videovina/static/amazon_s3";
    QString catsfarm = "/home/pancho/Documents/GitHub/cats_farm";
    QString localFolder = "/home/pancho/Desktop/renders";
    // -------------------------------

    QString slideshow = assets + "/slideshows";
    QString musicDir = assets + "/music";

    // Datos recividos del servidor principal
    QString user = recv[0].toString();
    QString projectType = recv[1].toString();
    QString projectName = recv[2].toString();
    bool proxy = recv[3].toString().toInt();
    // -------------------------------------

    // Abre el project json del el respaldo de as3
    QString vvProjectDir = as3 + "/" + user + "/projects/" + projectName;
    QString vvProjectJson = vvProjectDir + "/project.json";
    QJsonObject vvProject = jread(vvProjectJson);
    // --------------------------------------------

    // Directorios locales
    QString userDir = localFolder + "/" + user;
    QString projectDir = userDir + "/" + projectName;
    QString project;
    if (proxy)
        project = projectDir + "/" + projectType + "_proxy.aep";
    else
        project = projectDir + "/" + projectType + ".aep";
    // ---------------------------------

    print("copiando footage...");

    // Cambia la ruta de la musica y la guarda el el project.json
    QString song = vvProject["song"].toString() + ".mp3";
    vvProject["songPath"] = musicDir + "/" + song;
    // -----------------------------------

    // guarda ruta de los proyectos slideshows
    vvProject["slideshowPath"] = slideshow;
    // -----------------------------------
    vvProject["proxy"] = proxy;

    os::makedirs(projectDir + "/renders");

    // Copia la plantilla del proyecto en el directorio local
    QString originalProject;
    if (proxy)
        originalProject = slideshow + "/" + projectType + "/" + projectType + "_proxy.aep";
    else
        originalProject = slideshow + "/" + projectType + "/" + projectType + ".aep";
    os::copy(originalProject, project);
    // -------------------------------------------

    // Copia footage, project.json de amazon S3 y lo copia en el directorio compartido local
    os::copydir(vvProjectDir + "/footage", projectDir);
    jwrite(projectDir + "/project.json", vvProject);
    // -------------------------------------------

    // Permisos de carpeta de proyecto
    os::sh("chmod 777 -R " + projectDir);
    // -------------------------------------

    // Modifica el proyecto de after effect con los datos del proyecto del usuario
    QString vina2ae = catsfarm + "/modules/videovina/vina2ae.jsx";
    QString afterfx = "/opt/AE9.0/AfterFX.exe";
    QString cmd = "wine " + afterfx + " -noui -s \"var aep = '" + project + "';//@include '" + vina2ae + "';\"";
    print("start project");
    os::sh(cmd);
    print("end.");
    // ---------------------------------------------------

    // genera la ruta base de la salida del mov, no es completa por que despues en el render
    // hay que ponerle el numero del video y la extencion por cada tarea para poder hacer el concat
    QString output = projectDir + "/renders/" + projectName + "/" + projectName;
    // ---------------------------------------------------

    // Lee los datos de salida del script vina2ae.jsx
    QString submitJson = projectDir + "/submit.json";
    QJsonObject submit = jread(submitJson); // aumento de ram cuando no encuetra el archivo json ( ¡revisar)
    // ---------------------------------------------

    // aveces en las pruebas cuando vina2ae.jsx da un error no crea el submit.json y no lo puede leer
    // por eso si no hay informacion retorna para que no de conflicto despues
    if (submit.empty())
        return "";
    // ------------------------------------

    QString job_name = user + "-" + projectName;
    QString server = "None";
    QString server_group = "videovina";
    int first_frame = 1;
    int last_frame = submit["last_frame"].toInt();
    int task_size = 30;
    int priority = 1;
    bool suspend = false;
    QString comment = "VideoVina Render";
    QString software = "AE";
    QString extra = output;
    QString _system = "linux";
    int instances;
    if (proxy)
        instances = 7;
    else
        instances = 3;
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

    make_job(data);

    return "";
}
