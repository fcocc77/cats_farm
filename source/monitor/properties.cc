#include <QVBoxLayout>

#include <properties.h>

properties_class::properties_class(log_class *_log, options_class *_options,
                                   settings_class *_settings)

    : log(_log)
    , options(_options)
    , settings(_settings)
{
    setup_ui();

    log->hide();
    options->hide();

    current_widget = "settings";
}

void properties_class::setup_ui()
{

    QVBoxLayout *properties_layout = new QVBoxLayout();
    this->setLayout(properties_layout);

    properties_layout->addWidget(log);
    properties_layout->addWidget(options);
    properties_layout->addWidget(settings);
}

void properties_class::switch_widget(QString widget_name)
{

    if (widget_name == current_widget)
    {
        bool visible;
        if (widget_name == "log")
        {
            visible = !log->isVisible();
            log->setVisible(visible);
        }

        else if (widget_name == "options")
        {
            visible = !options->isVisible();
            options->setVisible(visible);
        }

        else if (widget_name == "settings")
        {
            visible = !settings->isVisible();
            settings->setVisible(visible);
        }

        this->parentWidget()->setVisible(visible);
    }
    else
    {
        log->hide();
        options->hide();
        settings->hide();

        if (widget_name == "log")
            log->show();

        else if (widget_name == "options")
            options->show();

        else if (widget_name == "settings")
            settings->show();

        current_widget = widget_name;

        this->parentWidget()->show();
    }
}
