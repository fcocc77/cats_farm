#include <QVBoxLayout>

#include <properties.h>

properties_class::properties_class(log_class *_log, options_class *_options,
                                   settings_class *_settings, submit *__submit)

    : log(_log)
    , options(_options)
    , settings(_settings)
    , _submit(__submit)
{
    setup_ui();

    log->hide();
    options->hide();
    _submit->hide();

    current_widget = "settings";
}

void properties_class::setup_ui()
{

    QVBoxLayout *properties_layout = new QVBoxLayout();
    this->setLayout(properties_layout);

    properties_layout->addWidget(log);
    properties_layout->addWidget(options);
    properties_layout->addWidget(settings);
    properties_layout->addWidget(_submit);
}

bool properties_class::switch_widget(QString widget_name)
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
        else if (widget_name == "submit")
        {
            visible = !_submit->isVisible();
            _submit->setVisible(visible);
        }

        this->parentWidget()->setVisible(visible);

        return visible;
    }
    else
    {
        log->hide();
        options->hide();
        settings->hide();
        _submit->hide();

        if (widget_name == "log")
            log->show();

        else if (widget_name == "options")
            options->show();

        else if (widget_name == "settings")
            settings->show();

        else if (widget_name == "submit")
            _submit->show();

        current_widget = widget_name;

        this->parentWidget()->show();
    }

    return true;
}
