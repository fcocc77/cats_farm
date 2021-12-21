#include "video.h"
#include "../global/global.h"
#include "os.h"
#include "util.h"

namespace video
{

void concat(QString folder, int task_size, QString format)
{
    QString ffmpeg =
        _linux ? "ffmpeg" : VINARENDER_PATH + "/os/win/ffmpeg/ffmpeg.exe";

    QString log_method = _linux ? " > " : " &> ";

    QString name = os::basename(folder);
    QString dir_movie = os::dirname(folder);
    QString list = dir_movie + "/list_" + name;

    QString movie_list = "";
    QStringList dir_list = os::listdir(folder);

    float frame_rate = get_meta_data(dir_list[0]).frame_rate;
    float piece_duration = (float)task_size / frame_rate;

    sort(dir_list.begin(), dir_list.end());
    for (auto i : dir_list)
    {
        QString ext = i.split(".").last();
        if (ext == "mov" || ext == "mp4")
        {
            movie_list += "file '" + i + "'\n";
            movie_list += "duration " + QString::number(piece_duration) + "\n";
        }
    }

    fwrite(list, movie_list);

    QString movie = dir_movie + "/" + name + "." + format;

    if (_win32)
        list.replace("/", "\\");

    QString cmd = "%1 -y -f concat -safe 0 -i \"%2\" -c:v copy \"%3\"";
    cmd = cmd.arg(ffmpeg, list, movie);

    os::sh(cmd);
    os::remove(list);

    if (os::isfile(movie))
        os::remove(folder);
}

meta get_meta_data(QString file)
{
    QString ffprobe = "/usr/bin/ffprobe";

    QString meta_data =
        os::sh(ffprobe +
               " -hide_banner -loglevel fatal -show_error -show_format "
               "-show_streams -print_format json \"" +
               file + "\"");

    QJsonObject _meta_data =
        jofs(meta_data).value("streams").toArray()[0].toObject();

    meta _meta;

    QStringList _frame_rate =
        _meta_data.value("r_frame_rate").toString().split('/');

    _meta.frame_rate = _frame_rate[0].toFloat() / _frame_rate[1].toFloat();
    _meta.frames = _meta_data.value("nb_frames").toString().toInt();

    return _meta;
}

float frame_to_seconds(int frame, float frame_rate)
{
    return float(frame) / frame_rate;
}

} // namespace video
