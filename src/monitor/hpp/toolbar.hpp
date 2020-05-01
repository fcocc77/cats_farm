#ifndef TOOLBAR_HPP
#define TOOLBAR_HPP

#include "includes.hpp"

#include "global.hpp"
#include "jobs.hpp"
#include "update.hpp"
#include "shared_variables.hpp"

class toolbar_class : public QToolBar
{
private:
    global_class *global;
    jobs_class *jobs;
    update_class *update;
    shared_variables *shared;
    QDockWidget *settings_dock;

    void setup_ui();
    void connections();
    void load_zones();

    QPushButton *resume;
    QPushButton *suspend;
    QPushButton *settings;

public:
    toolbar_class(
        global_class *_global,
        jobs_class *_jobs,
        update_class *_update,
        shared_variables *_shared,
        QDockWidget *_settings_dock);
    ~toolbar_class();
};

#endif //TOOLBAR_HPP
