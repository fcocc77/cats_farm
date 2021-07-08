#ifndef SETTINGS_SERVER_H
#define SETTINGS_SERVER_H

#include <QWidget>

#include "text_knob.h"
#include "shared_variables.h"

class settings_server : public QWidget
{
private:
    shared_variables *shared;

    text_knob *manager_host;

public:
    settings_server(shared_variables *shared);

    void save();
    void restore();
};
#endif // SETTINGS_SERVER_H
