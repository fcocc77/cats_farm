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
    void ok();

    shared_variables *shared;

public:
    settings_class(
        Ui::MainWindow *_ui,
        shared_variables *_shared);
    ~settings_class();
    void update(QString host);
};

#endif // SETTING_HPP