#ifndef SETTINGS_SUBMIT_H
#define SETTINGS_SUBMIT_H

#include <QWidget>

#include "text_knob.h"
#include "shared_variables.h"

class settings_submit : public QWidget
{
private:
    shared_variables *shared;

    text_knob *manager_host;

public:
    settings_submit(shared_variables *shared);

    void save();
    void restore();
};
#endif // SETTINGS_SUBMIT_H
