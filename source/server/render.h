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
    QList<QString> job_system;
    QList<bool> task_kill, render_instance;
    QList<QJsonObject> software_data;
    QMutex *mutex;

    render_class(QMutex *_mutex);

    QString render_task(QJsonObject _task);
    QString qprocess(QString cmd, int ins = -1, int timeout = -1);

    void get_correct_path(QString filename, QString *src, QString *dst);

    QString get_executable(QString software) const;

    bool nuke(int ins);
    bool maya(int ins);
    bool houdini(int ins);
    bool ffmpeg(int ins);
    bool vinacomp(int ins);
};

#endif // RENDER_HPP
