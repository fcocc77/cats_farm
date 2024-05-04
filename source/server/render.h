// Author: Francisco Contreras
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#ifndef RENDER_HPP
#define RENDER_HPP

#include "../utils/threading.h"
#include <QMutex>
#include <util.h>

class render_class : public QObject
{
private:
    struct instance_data
    {
        int first_frame;
        int last_frame;
        int pid;
        bool task_kill;
        bool render_instance;
        QJsonObject software_data;
        QString job_system;
        QString software;
    };

    QMutex *mutex;

    QString qprocess(QString cmd, int ins, bool *error);

    void get_correct_path(QString filename, QString *src, QString *dst);

    QString get_executable(QString software) const;

    bool nuke(int ins);
    bool maya(int ins);
    bool houdini(int ins);
    bool ffmpeg(int ins);
    bool vinacomp(int ins);

    void log_save(int ins, QString cmd, QString log);

public:
    render_class(QMutex *_mutex);

    QString render_task(QJsonObject _task);
    QJsonObject settings_from_manager;
    QList<instance_data> idata;
};

#endif // RENDER_HPP
