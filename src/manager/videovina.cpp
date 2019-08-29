#include "manager.h"

QString manager::videovina(QJsonArray recv)
{

    QString user = recv[0].toString();
    QString projectName = recv[1].toString();
    QString projectType = projectName;

    QString slideshow = "/home/pancho/Documents/GitHub/videovina/private/slideshows";
    QString as3 = "/home/pancho/Documents/GitHub/videovina/static/amazon_s3";

    QString vvProjectDir = as3 + "/" + user + "/projects/" + projectName;
    QString vvProject = vvProjectDir + "/project.json";

    QString localFolder = "/home/pancho/Desktop/renders";

    QString userDir = localFolder + "/" + user;
    QString projectDir = userDir + "/" + projectName;

    os::makedirs(userDir);

    // Copia la plantilla del proyecto en el directorio local
    //os::copydir(slideshow + "/" + projectType, projectDir);
    // -------------------------------------------

    // Copia footage de amazon S3 y lo copia en el directorio compartido local
    //os::copydir(vvProjectDir + "/footage", projectDir);
    // -------------------------------------------

    QString project = projectDir + "/Ink Slideshow_v05.aep";
    project = "/home/pancho/Documents/GitHub/videovina/private/slideshows/wedding_01/Ink Slideshow_v05.aep";

    QString vina2aeJsx = "/home/pancho/Documents/GitHub/cats_farm/modules/videovina/vina2ae.jsx";
    QString vina2ae = fread(vina2aeJsx);

    // Cambia las comillas dobles por las simples en el vina2ae.jsx, porque para ejecutar un script en after effect
    // tienen que estar en comillas dobles
    vina2ae.replace("\"", "'");
    // -------------------------------------

    vina2ae.replace("aepProject", project);
    vina2ae.replace("\n", "");

    QString afterfx = "/opt/AE9.0/AfterFX.exe";

    QString cmd = "wine " + afterfx + " -s \"" + vina2ae + "\"";

    fwrite("/home/pancho/Desktop/yes.jsx", cmd);

    os::system(cmd);

    QString job_name = recv[0].toString();
    QString server = "None";
    QString server_group = "videovina";
    int first_frame = 1;
    int last_frame = 20;
    int task_size = 2;
    int priority = 1;
    bool suspend = false;
    QString comment = "Render de VideoVina";
    QString software = "AE";

    QString extra = "";
    QString _system = "linux";
    int instances = 1;
    QString render = "render";

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
