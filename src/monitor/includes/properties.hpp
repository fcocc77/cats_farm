#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

#include <includes.hpp>

#include "log.hpp"
#include "options.hpp"
#include "settings.hpp"

class properties_class : public QWidget
{

private:
    log_class *log;
    options_class *options;
    settings_class *settings;

    void setup_ui();
    QString current_widget;

public:
    properties_class(
        log_class *_log,
        options_class *_options,
        settings_class *_settings);

    void switch_widget(QString widget_name);
};

#endif // PROPERTIES_HPP