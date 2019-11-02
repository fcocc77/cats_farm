#ifndef TOOLBAR_HPP
#define TOOLBAR_HPP

#include "includes.hpp"

#include "global.hpp"
#include "jobs.hpp"

class toolbar_class : public QObject
{
private:
    Ui::MainWindow *ui;
    global_class *global;
    jobs_class *jobs;

    void connections();
    void property();

public:
    toolbar_class(
        Ui::MainWindow *_ui,
        global_class *_global,
        jobs_class *_jobs);
    ~toolbar_class();
};

#endif //TOOLBAR_HPP
