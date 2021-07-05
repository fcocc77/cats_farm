#include <QVBoxLayout>
#include <QScrollArea>

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
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);

    QWidget *main_widget = new QWidget();
    QVBoxLayout *main_layout = new QVBoxLayout(main_widget);

    main_layout->addWidget(log);
    main_layout->addWidget(options);
    main_layout->addWidget(settings);
    main_layout->addWidget(_submit);

    QScrollArea *scroll_area = new QScrollArea();
    scroll_area->setWidget(main_widget);
    scroll_area->setWidgetResizable(true);

    layout->addWidget(scroll_area);
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

        this->setVisible(visible);

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

        this->show();
    }

    return true;
}
