#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "../hpp/includes.hpp"
#include "../hpp/global.hpp"
#include "../hpp/jobs.hpp"

class main_menu_class
{
private:
    Ui::MainWindow *ui;
    void connections();

    global_class *global;
    jobs_class *jobs;

public:
    main_menu_class(Ui::MainWindow *_ui,
                    global_class *_global,
                    jobs_class *_jobs);
    ~main_menu_class();
};

#endif //MAIN_MENU_HPP