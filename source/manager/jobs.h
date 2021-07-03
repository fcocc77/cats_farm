#ifndef JOBS_H
#define JOBS_H

#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>

#include "tasks.h"
#include "jobs_struct.h"

class jobs
{

private:
    void *_manager;

    QList<job_struct *> *items;
    void job_delete(QString job_name);

public:
    jobs(void *manager);

    inline QList<job_struct *> *get_items();
    void make_job(QJsonObject __job);
    void update();
    job_struct *get_job(QString name);

    void job_action(QJsonArray pks);
    QString job_options(QJsonArray pks);
    QString job_log_action(QString pks);
};

inline QList<job_struct *> *jobs::get_items()
{
    return items;
}

#endif // JOBS_H
