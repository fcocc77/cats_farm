#ifndef RENDER_H
#define RENDER_H

#include <QString>
#include <QMutex>

#include "servers_struct.h"
#include "jobs_struct.h"
#include "jobs.h"
#include "servers.h"
#include "groups.h"

class renderer
{
private:
    void *_manager;
    QMutex *mutex;
    jobs *_jobs;
    servers *_servers;
    groups *_groups;

    bool reset_render;

    void render_job();
    void render_task(server_struct *server, inst_struct *instance,
                     job_struct *job);

    void get_correct_path(QString filename, QString *src, QString *dst);
    void nuke_completed(job_struct *job);
    void vinarender_completed(job_struct *job);
    void ffmpeg_completed(job_struct *job);


public:
    renderer(void *manager, QMutex *mutex, jobs *_jobs, servers *_servers,
             groups *_tasks);

    inline void to_reset_render();
};

inline void renderer::to_reset_render()
{
    reset_render = true;
}

#endif // RENDER_H
