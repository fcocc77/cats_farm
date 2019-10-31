#ifndef TOOLBAR_HPP
#define TOOLBAR_HPP

#include "includes.hpp"

#include "global.hpp"

class toolbar_class
{
private:
    Ui::MainWindow *ui;
    global_class *global;

    void property();

public:
    toolbar_class(
        Ui::MainWindow *_ui,
        global_class *_global);
    ~toolbar_class();
};

#endif //TOOLBAR_HPP
