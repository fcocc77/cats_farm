#ifndef STRUCT_TRANSFORM_H
#define STRUCT_TRANSFORM_H

#include <QJsonObject>

#include "jobs.h"
#include "servers.h"
#include "groups.h"

QJsonObject struct_to_json(jobs *_jobs, servers *_servers, groups *_groups);

void json_to_struct(QJsonObject info, jobs *_jobs, servers *_servers,
                    groups *_groups);

#endif // STRUCT_TRANSFORM_H
