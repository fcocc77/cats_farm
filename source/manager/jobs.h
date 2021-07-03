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
    QJsonObject get_options(QString job_name) const;
    void write_options(QString job_name, QJsonObject options);

public:
    jobs(void *manager);

    inline QList<job_struct *> *get_items();
    void make_job(QJsonObject __job);
    void update();
    job_struct *get_job(QString name) const;

    void job_action(QJsonArray pks);
    QString update_options(QJsonObject pks);
    QString job_log_action(QString pks);
};

inline QList<job_struct *> *jobs::get_items()
{
    return items;
}

#endif // JOBS_H
