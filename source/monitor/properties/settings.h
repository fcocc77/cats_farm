#ifndef SETTING_HPP
#define SETTING_HPP

#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QWidget>

#include "settings_manager.h"
#include "settings_monitor.h"
#include "settings_server.h"
#include "shared_variables.h"
#include "text_knob.h"

class settings_class : public QWidget
{
private:
    void setup_ui();
    void connections();

    QWidget *_monitor;
    shared_variables *shared;

    settings_manager *_settings_manager;
    settings_monitor *_settings_monitor;
    settings_server *_settings_server;

    QPushButton *cancel_button;
    QPushButton *save_button;

public:
    settings_class(QWidget *_monitor, shared_variables *_shared);
    ~settings_class();

    inline settings_manager *get_manager() const;
    inline settings_monitor *get_monitor() const;
};

inline settings_manager *settings_class::get_manager() const
{
    return _settings_manager;
}

inline settings_monitor *settings_class::get_monitor() const
{
    return _settings_monitor;
}

#endif // SETTING_HPP
