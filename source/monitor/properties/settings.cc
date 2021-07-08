#include <QVBoxLayout>

#include "../global/global.h"
#include "main_window.h"
#include "settings.h"
#include "submit_global.h"
#include "tcp.h"
#include "toolbar.h"
#include "util.h"

settings_class::settings_class(QWidget *__monitor, shared_variables *_shared)
    : _monitor(__monitor)
    , shared(_shared)
{
    setup_ui();
    connections();

    _settings_monitor->restore();
    _settings_server->restore();
}

settings_class::~settings_class() {}

void settings_class::setup_ui()
{
    this->setObjectName("settings");
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->setMargin(0);

    QWidget *buttons_widget = new QWidget;
    QHBoxLayout *buttons_layout = new QHBoxLayout(buttons_widget);
    buttons_layout->setMargin(0);

    cancel_button = new QPushButton("Cancel");
    save_button = new QPushButton("Save");

    _settings_manager = new settings_manager(shared);
    _settings_monitor = new settings_monitor(shared);
    _settings_server = new settings_server(shared, this);

    settings_tab = new QTabWidget();

    // Layout
    settings_tab->addTab(_settings_monitor, "Monitor");
    settings_tab->addTab(_settings_manager, "Manager");
    settings_tab->addTab(_settings_server, "Server");

    buttons_layout->addWidget(cancel_button);
    buttons_layout->addWidget(save_button);

    main_layout->addWidget(settings_tab);
    main_layout->addWidget(buttons_widget);
}

void settings_class::connections()
{
    connect(cancel_button, &QPushButton::clicked, this, [this]() {
        static_cast<monitor *>(_monitor)->get_toolbar()->hide_properties();
    });

    connect(save_button, &QPushButton::clicked, this, [this]() {
        int tab_index = settings_tab->currentIndex();

        if (tab_index == 0)
            _settings_monitor->save();

        else if (tab_index == 1)
            _settings_manager->save();

        else if (tab_index == 2)
            _settings_server->save();
    });
}
