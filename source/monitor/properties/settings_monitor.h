#ifndef SETTINGS_MONITOR_H
#define SETTINGS_MONITOR_H

#include <QWidget>

#include "text_knob.h"
#include "shared_variables.h"

class settings_monitor : public QWidget
{
private:
    shared_variables *shared;

    text_knob *zones_edit;

public:
    settings_monitor(shared_variables *shared);
    void save();
    void restore();
};
#endif // SETTINGS_MONITOR_H
