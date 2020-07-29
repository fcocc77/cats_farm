#include "manager.hpp"

void manager::send_to_render(QString extra)
{
    // espera que los proyectos natron esten creados con los
    // datos del proyecto del usuario para poder enviarlo a renderizar

    QJsonObject _extra = jofs(extra);

    QString user = _extra["user"].toString();
    QString project_name = _extra["project_name"].toString();

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

void manager::post_render(QJsonObject extra, int last_frame)
{
    QString output_file = extra["output"].toString();
    QString output_dir = os::dirname(output_file);
    QString output_name = os::basename(output_file);

    output_name = output_name.split(".")[0];

    QString output = output_dir + "/" + output_name + "_audio.mp4";
    QString song = extra["song"].toString();

    float frame_rate = 30.0;
    float duration = last_frame / frame_rate;
    QString _duration = QString::number(duration);

    int fade_duration = 4;
    float fade_start = duration - fade_duration;

    QString audio_filter = "-filter:a \"afade=t=out:st=" + QString::number(fade_start) + ":d=" + QString::number(fade_duration) + "\"";
    QString cmd = "ffmpeg -y -i \"" + output_file + "\" -i \"" + song + "\" -c:v copy " + audio_filter + " -t " + _duration + " \"" + output + "\"";

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
        {"user_id", user_id},
        {"project_name", project_name},
        {"project_type", project_type},
        {"format", format}};

    QString _data = jots(data).replace("\"", "'");

    QString cmd = natron_renderer + " " + api + " \"" + _data + "\"";
    os::system(cmd);
}
