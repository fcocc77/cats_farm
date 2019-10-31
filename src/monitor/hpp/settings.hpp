#ifndef SETTING_HPP
#define SETTING_HPP

#include "includes.hpp"

class settings_class
{
private:
    Ui::MainWindow *ui;
    void property();

public:
    settings_class(
        Ui::MainWindow *_ui);
    ~settings_class();
};

#endif // SETTING_HPP