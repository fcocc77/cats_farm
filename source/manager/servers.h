// Author: Francisco Jose Contreras Cuevas
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#ifndef SERVERS_H
#define SERVERS_H

#include <QString>
#include <QList>
#include <QJsonArray>

#include "servers_struct.h"

class servers
{
private:
    void *_manager;

    QList<server_struct *> *items;
    QJsonObject *settings;

    void server_set_state(server_struct *server, bool state);

public:
    servers(void *manager);

    QString update_server_thread(QJsonObject recv);
    void reset_all_servers();
    server_struct *get_server(QString name);

    inline QList<server_struct *> *get_items();
    QString server_action(QJsonArray pks);
    void update();
};

inline QList<server_struct *> *servers::get_items()
{
    return items;
}


#endif // SERVERS_H
