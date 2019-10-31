#ifndef LOG_HPP
#define LOG_HPP

#include "includes.hpp"

class log_class
{
private:
    Ui::MainWindow *ui;
    void property();

public:
    log_class(
        Ui::MainWindow *_ui);
    ~log_class();
};

#endif // LOG_HPP