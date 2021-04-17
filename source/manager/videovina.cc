#include <manager.h>

void manager::sample_render(QString video, int frame, int index)
{

    float frame_rate = 30.0;

    QString start_second = QString::number(frame / frame_rate);

    QString samples_folder = os::dirname(video) + "/samples";
    os::makedirs(samples_folder);

    QString output =
        samples_folder + "/sample_" + QString::number(index) + ".jpg";

    QString cmd = "ffmpeg -y -i \"" + video + "\" -ss " + start_second +
                  " -vf scale=640:360 -vframes 1 -q:v 2 \"" + output + "\"";
    os::sh(cmd);
}

void manager::samples_export(QString video, QJsonArray ranges)
{

    for (int i = 0; i < ranges.size(); i++)
    {
        QJsonArray range = ranges[i].toArray();
        int first_frame = range[0].toInt();
        int last_frame = range[1].toInt();

        int center_frame = (first_frame + last_frame) / 2;

        sample_render(video, center_frame, i);
    }
}

void manager::send_to_render(job_struct *job)
{
    // espera que los proyectos natron esten creados con los
    // datos del proyecto del usuario para poder enviarlo a renderizar

    QJsonObject _extra = jofs(job->extra);

    QString user = _extra["user"].toString();
    QString project_name = _extra["project_name"].toString();
    QString _module = _extra["module"].toString();

    QString natron_renderer = "/opt/Natron2/bin/NatronRenderer";
    QString api = path + "/modules/natron/api.py";

    QJsonObject data = {{"action", "send_to_render"},
                        {"user", user},
                        {"project_name", project_name}};

    QString _data = jots(data).replace("\"", "'");

    QString cmd = natron_renderer + " " + api + " \"" + _data + "\"";
    os::sh(cmd);

    if (_module == "production_ntp")
        job_delete(job->name);
}

void manager::post_render(QJsonObject extra, int last_frame, QString job_name)
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

    QString audio_filter =
        "-filter:a \"afade=t=out:st=" + QString::number(fade_start) +
        ":d=" + QString::number(fade_duration) + "\"";
    QString cmd = "ffmpeg -y -i \"" + output_file + "\" -i \"" + song +
                  "\" -c:v copy " + audio_filter + " -t " + _duration + " \"" +
                  output + "\"";

    os::sh(cmd);

    samples_export(output, extra["ranges"].toArray());

    // copia el video con audio, a la carpeta s3 del usuario
    QString project_name = extra["project_name"].toString();
    QString user_id = extra["user_id"].toString();
    QString s3_project_folder =
        as3 + "/public/" + user_id + "/projects/" + project_name;
    QString s3_video_path = s3_project_folder + "/" + project_name + ".mp4";
    os::copy(output, s3_video_path);

    // copia las muestras a la carpeta s3
    QString samples_folder = output_dir + "/samples";

    os::copydir(samples_folder, s3_project_folder);

    job_delete(job_name);
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

    QJsonObject data = {{"action", "create_multi_project"},
                        {"user", user},
                        {"user_id", user_id},
                        {"project_name", project_name},
                        {"project_type", project_type},
                        {"format", format}};

    QString _data = jots(data).replace("\"", "'");

    QString cmd = natron_renderer + " " + api + " \"" + _data + "\"";
    os::sh(cmd);
}
