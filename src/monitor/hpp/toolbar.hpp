#ifndef TOOLBAR_HPP
#define TOOLBAR_HPP

#include "includes.hpp"

#include "global.hpp"
#include "jobs.hpp"
#include "update.hpp"

class toolbar_class : public QObject
{
private:
    Ui::MainWindow *ui;
    global_class *global;
    jobs_class *jobs;
    update_class *update;

    void connections();
    void property();
    void load_zones();

public:
    toolbar_class(
        Ui::MainWindow *_ui,
        global_class *_global,
        jobs_class *_jobs,
        update_class *_update);
    ~toolbar_class();
};

#endif //TOOLBAR_HPP
