#include "manager.hpp"

void manager::send_to_render(QString extra)
{
    // espera que los proyectos natron esten creados con los
    // datos del proyecto del usuario para poder enviarlo a renderizar

    QJsonArray _extra = jafs(extra);
    QJsonObject slide = _extra[0].toObject();

    QString user = slide["user"].toString();
    QString project_name = slide["project_name"].toString();

    QString natron_renderer = "/opt/Natron2/bin/NatronRenderer";
    QString api = path + "/modules/natron/api.py";

    QJsonObject data = {
        {"action", "send_to_render"},
        {"user", user},
        {"project_name", project_name}};

    QString _data = jots(data).replace("\"", "'");

    QString cmd = natron_renderer + " " + api + " \"" + _data + "\"";
    os::system(cmd);
}

void manager::videovina(QJsonArray recv)
{
    // Datos recividos del servidor principal
    QString user = recv[0].toString();
    QString user_id = recv[1].toString();
    QString project_type = recv[2].toString();
    QString project_name = recv[3].toString();
    int format = recv[4].toString().toInt();
    // -------------------------------------

    QString natron_renderer = "/opt/Natron2/bin/NatronRenderer";
    QString api = path + "/modules/natron/api.py";

    QJsonObject data = {
        {"action", "create_multi_project"},
        {"user", user},
        {"project_name", project_name},
        {"project_type", project_type},
        {"format", format}};

    QString _data = jots(data).replace("\"", "'");

    QString cmd = natron_renderer + " " + api + " \"" + _data + "\"";
    os::system(cmd);
}
