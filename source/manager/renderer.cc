#include <unistd.h>

#include "renderer.h"
#include "path_utils.h"
#include "threading.h"
#include "video.h"
#include "util.h"
#include "manager.h"
#include "tcp.h"
#include "../global/global.h"

renderer::renderer(void *__manager, QMutex *_mutex, jobs *__jobs,
                   servers *__servers, groups *__groups)

    : _manager(__manager)
    , mutex(_mutex)
    , _jobs(__jobs)
    , _servers(__servers)
    , _groups(__groups)
    , reset_render(false)
{

    threading(&renderer::render_job, this);
}

void renderer::render_job()
{
    auto *jobs_items = _jobs->get_items();
    auto *groups_items = _groups->get_items();
    auto *servers_items = _servers->get_items();

    while (1)
    {
        mutex->lock();
        reset_render = false;

        // lee las listas del trabajo creadas y las ordena segun prioridad
        std::sort(jobs_items->begin(), jobs_items->end(),
                  [](job_struct *a, job_struct *b) {
                      bool cmp;
                      cmp = a->priority < b->priority;

                      if (a->priority == b->priority)
                          cmp = a->submit_start < b->submit_start;

                      return cmp;
                  });

        // si el trabajo esta en cola se manda a render
        for (auto job : *jobs_items)
        {
            // hace una lista con los servidores listos para renderizar
            QStringList server_list;
            for (auto sg : job->server_group)
            {
                if (not groups_items->empty())
                {
                    auto group = _groups->get_group(sg);

                    for (auto s : group->server)
                        server_list.push_back(s->name);
                }
            }

            for (auto server : *servers_items)
            {
                // Checkea si esl servidor esta prendido y tambien si esta en
                // el grupo del jobs
                bool server_ok = 0;
                if (server_list.contains(server->name) and
                    server->status != "absent")
                    server_ok = 1;

                for (int ins = 0; ins < server->max_instances; ins++)
                {
                    // si la instancia esta dentro del trabajo renderea
                    bool instance_ok = 0;
                    for (int i = 0; i < job->instances; i++)
                        if (i == ins)
                            instance_ok = 1;

                    if (server_ok && instance_ok)
                    {
                        if (job->status == "Queue" ||
                            job->status == "Rendering..." ||
                            job->status == "Failed")
                        {
                            // Veta los servers que fallaron mas de dos
                            // veces
                            int vetoed_times = 0;
                            for (auto s : job->vetoed_servers)
                                if (server->name == s)
                                    vetoed_times++;

                            if (not(vetoed_times >= 10))
                            { // este numero es la cantidad de veces que
                              // puede fallar el servidor antes que se
                              // bloquee

                                if (job->waiting_task)
                                {
                                    auto instance = server->instances[ins];
                                    int active = instance->status;
                                    if (not active)
                                    {
                                        instance->status = 1;
                                        threading(&renderer::render_task, this,
                                                  server, instance, job);
                                    }
                                }
                            }
                        }
                    }

                    if (reset_render)
                        break;
                }

                if (reset_render)
                    break;
            }

            if (reset_render)
                break;
        }

        mutex->unlock();
        sleep(1);
    }
}

void renderer::render_task(server_struct *server, inst_struct *instance,
                          job_struct *job)
{
    instance->reset = 0;

    QString software = job->software.toLower();

    bool Completed = false;
    QString result = "";

    // renderea las tareas por cada trabajo, solo si status es igual a waiting
    for (auto task : job->task)
    {
        mutex->lock();
        int first_frame = task->first_frame;
        int last_frame = task->last_frame;
        QString status = task->status;

        // pone en la instancia que job se esta rendereando
        instance->job_task = job->name + " : " + task->name;

        if (instance->reset || server->status == "absent")
        {
            mutex->unlock();
            break;
        }

        // envia la tarea al servidor para que la procese, si es que el status
        // es waiting
        if (status == "waiting")
        {
            // pone en activo la tarea
            task->status = "active";
            job->active_task++;
            task->server =
                server->name + ":" + QString::number(instance->index);
            int time1 = time(0);

            job->status = "Rendering...";
            mutex->unlock();

            // Envia a renderar la tarea al servidor que le corresponde
            QJsonObject pks = {
                {"software", software},
                {"software_data", job->software_data},
                {"first_frame", first_frame},
                {"last_frame", last_frame},
                {"job_system", job->system},
                {"instance", instance->index}
            };

            QString result =
                tcpClient(server->host, SERVER_PORT, jats({0, pks}));

            if (not(result == "ok"))
            {
                if (result == "kill")
                {
                    mutex->lock();
                    int active_task = job->active_task - 1;
                    if (active_task >= 0)
                        job->active_task = active_task;

                    task->status = "waiting";
                    task->time = "...";
                    task->server = "...";
                    mutex->unlock();
                    break;
                }

                else
                { // if failed
                    mutex->lock();
                    job->vetoed_servers.push_back(server->name);

                    job->failed_task = 1;
                    job->errors++;

                    int active_task = job->active_task - 1;
                    if (active_task >= 0)
                        job->active_task = active_task;

                    task->status = "failed";
                    mutex->unlock();
                    sleep(4);
                    mutex->lock();
                    task->status = "waiting";
                    task->time = "...";
                    task->server = "...";
                    mutex->unlock();

                    break;
                }
            }

            mutex->lock();
            int active_task = job->active_task - 1;
            if (active_task >= 0)
                job->active_task = active_task;

            task->status = "finished";

            int seconds = time(0) - time1;
            task->time = timeStruct(seconds);

            auto tasks = job->tasks;
            int before_progres = job->progres;
            int after_progres;

            // cuando el valor del progreso ya es igual al total de las tareas,
            // no le suma 1 al progreso, si no el progreso queda arriba de 100%
            if (not(tasks == before_progres))
            {
                after_progres = before_progres + 1;
                job->progres = after_progres;
            }
            else
                after_progres = before_progres;

            if (tasks == after_progres)
                Completed = true;

            mutex->unlock();
        }
        else
        {
            mutex->unlock();
        }
    }

    reset_render = true;
    mutex->lock();
    if (result == "failed")
    {
        instance->status = 3;
        job->status = "Failed";
    }

    else
    {
        // cuando uno pone el server inactivo se mantiene inactivo
        if (not(instance->status == 2))
        {
            instance->status = 0;
            instance->job_task = "...";
        }
    }
    mutex->unlock();
    if (Completed)
    {
        if (software == "nuke")
            nuke_completed(job);
        else if (software == "vinacomp")
            vinarender_completed(job);
        else if (software == "ffmpeg")
            ffmpeg_completed(job);

        QString submit_finish = currentDateTime(0);

        mutex->lock();
        job->submit_finish = submit_finish;
        job->status = "Completed";
        mutex->unlock();
    }
}

void renderer::get_correct_path(QString filename, QString *src, QString *dst)
{
    QJsonArray system_path = static_cast<manager *>(_manager)
                                 ->get_preferences()
                                 ->value("paths")
                                 .toObject()["system"]
                                 .toArray();

    QString src_path, dst_path, aux;
    for (QJsonValue p1 : system_path)
    {
        for (QJsonValue p2 : system_path)
        {
            aux = filename;
            aux.replace(p1.toString(), p2.toString());

            if (os::isfile(aux))
            {
                src_path = p1.toString();
                dst_path = p2.toString();
                break;
            }
        }

        if (os::isfile(aux))
            break;
    }

    *src = src_path;
    *dst = dst_path;
}

void renderer::nuke_completed(job_struct *job)
{
    QString filename = job->software_data["filename"].toString();
    QString ext = path_util::get_ext(filename);

    QString src_path, dst_path;
    get_correct_path(filename, &src_path, &dst_path);

    if (ext == "mov")
    {
        job->status = "Concatenate";

        // obtiene nombre de carpeta de renders
        QString _dirname = os::dirname(filename);
        QString _basename = os::basename(filename);
        _basename.replace(".mov", "");

        _dirname.replace(src_path, dst_path);

        if (os::isdir(_dirname))
            video::concat(_dirname + "/" + _basename, job->task_size);
    }
}

void renderer::vinarender_completed(job_struct *job)
{
}

void renderer::ffmpeg_completed(job_struct *job)
{
    job->status = "Concatenate";

    QString output_folder = job->software_data["output_folder"].toString();
    QString movie_name = job->software_data["movie_name"].toString();

    QString dir_file = output_folder + "/" + movie_name;

    if (os::isdir(dir_file))
        video::concat(dir_file, job->task_size);
}
