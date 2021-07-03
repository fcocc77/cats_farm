#ifndef GROUPS_H
#define GROUPS_H

#include <QString>
#include <QList>
#include <QJsonArray>

#include "groups_struct.h"

class groups
{
private:
    void *_manager;
    QList<group_struct *> *items;

public:
    groups(void *manager);

    inline QList<group_struct *> *get_items();
    void group_action(QJsonArray pks);
    void group_create(QJsonArray pks);
    group_struct *get_group(QString name);
    void update();
};

inline QList<group_struct *> *groups::get_items()
{
    return items;
}

#endif // GROUPS_H
