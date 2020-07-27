#include "manager.hpp"

void manager::wait_to_render()
{
    // espera que los proyectos natron esten creados con los
    // datos del proyecto del usuario para poder enviarlo a renderizar
    int timeout = 1000;
}

void manager::videovina(QJsonArray recv)
{

    QString slideshow = assets + "/slideshows";
    QString song_dir = assets + "/music";

    // Datos recividos del servidor principal
    QString user = recv[0].toString();
    QString user_id = recv[1].toString();
    QString project_type = recv[2].toString();
    QString project_name = recv[3].toString();
    bool proxy = recv[4].toString().toInt();
    // -------------------------------------

    QString natron_renderer = "/opt/Natron2/bin/NatronRenderer";
    QString api = path + "/modules/natron/api.py";

    QJsonObject data = {
        {"user", user},
        {"project_name", project_name},
        {"project_type", project_type}};

    QString _data = jots(data).replace("\"", "'");

    QString cmd = natron_renderer + " " + api + " \"" + _data + "\"";
    os::system(cmd);

    wait_to_render();
}
