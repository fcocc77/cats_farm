#ifndef SETTING_HPP
#define SETTING_HPP

#include "includes.hpp"

class settings_class : public QObject
{
private:
    Ui::MainWindow *ui;
    void property();
    void connections();
    void path_read();
    void path_write();

    QString manager_host;

public:
    settings_class(
        Ui::MainWindow *_ui);
    ~settings_class();
};

#endif // SETTING_HPP