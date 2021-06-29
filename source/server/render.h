#ifndef RENDER_HPP
#define RENDER_HPP

#include "../utils/threading.h"
#include <QMutex>
#include <util.h>

class render_class : public QObject
{
public:
    QJsonObject preferences;
    QList<int> first_frame, last_frame, pid;
    QList<QString> project, job_os, misc, render_node, src_path, dst_path;
    QList<bool> task_kill, render_instance;
    QMutex *mutex;

    render_class(QMutex *_mutex);

    QString render_task(QJsonArray recv);
    QList<QString> find_correct_path(QString file_path);
    QString qprocess(QString cmd, int ins = -1, int timeout = -1);

    QString get_executable(QString software) const;

    bool nuke(int ins);
    bool maya(int ins);
    bool houdini(int ins);
    bool ffmpeg(int ins);
    bool vinacomp(int ins);
};

#endif // RENDER_HPP
