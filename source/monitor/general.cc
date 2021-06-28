#include <QAction>
#include <QMenu>

#include "../global/global.h"
#include "general.h"
#include "util.h"
#include "toolbar.h"
#include "main_window.h"

general_class::general_class(QMainWindow *__monitor, shared_variables *_shared,
                             QWidget *_properties)

    : _monitor(__monitor)
    , shared(_shared)
    , properties(_properties)
{

    // General Action
    preferences_action = new QAction("Preferences");
    quit_action = new QAction("Quit");
    hide_action = new QAction("Hide");
    show_action = new QAction("Show App");
    properties_hide = new QAction("Hide Panels");
    update_style_action = new QAction("Update Style");

    auto icon = [=](QString name) {
        return QIcon(VINARENDER_PATH + "/resources/images/" + name +
                     "_normal.png");
    };

    preferences_action->setIcon(icon("settings"));
    quit_action->setIcon(icon("quit"));
    hide_action->setIcon(icon("hide"));
    properties_hide->setIcon(icon("hide"));

    notify_icon();
    connections();
    update_style();
}

void general_class::connections()
{
    connect(preferences_action, &QAction::triggered, this, [this]() {
        static_cast<monitor *>(_monitor)->get_toolbar()->switch_widget(
            "settings");
    });

    preferences_action->setShortcut(QString("S"));

    connect(quit_action, &QAction::triggered, this, [this]() {
        shared->app_close = true;
        _monitor->close();
    });

    connect(hide_action, &QAction::triggered, this, [this]() {
        if (_monitor->isVisible())
            _monitor->hide();
        else
            _monitor->show();
    });

    hide_action->setShortcut(QString("Ctrl+Q"));

    connect(show_action, &QAction::triggered, this,
            [this]() { _monitor->show(); });

    connect(properties_hide, &QAction::triggered, this,
            [this]() { properties->parentWidget()->hide(); });

    properties_hide->setShortcut(QString("Esc"));

    connect(update_style_action, &QAction::triggered, this,
            [=]() { update_style(true); });

    update_style_action->setShortcut(QString("Ctrl+R"));
}

void general_class::update_style(bool from_source_code)
{
    QString path = from_source_code ? VINARENDER_SOURCE : VINARENDER_PATH;
    QString style = fread(path + "/resources/css/style.css");

    _monitor->setStyleSheet("");
    _monitor->setStyleSheet(style.toStdString().c_str());

    // Titulo de ventana
    _monitor->setWindowTitle("VinaRender Monitor");
}

void general_class::notify_icon()
{
    notify = new QSystemTrayIcon(
        QIcon(VINARENDER_PATH + "/resources/icons/icon.png"), _monitor);

    connect(notify, &QSystemTrayIcon::messageClicked, this,
            [this]() { _monitor->show(); });

    connect(notify, &QSystemTrayIcon::activated, this,
            [this](QSystemTrayIcon::ActivationReason reason) {
                if (reason == QSystemTrayIcon::Trigger)
                {
                    // al clickear el icono muestra o oculta la ventana
                    // principal
                    if (_monitor->isVisible())
                        _monitor->hide();
                    else
                        _monitor->show();
                }
            });
    notify->show();

    QMenu *menu = new QMenu(_monitor);
    menu->addAction(show_action);
    menu->addAction(quit_action);

    notify->setContextMenu(menu);

    // Importa lista de jobs completados
    for (QJsonValue job : jafs(fread(VINARENDER_PATH + "/log/trayIcon")))
        completed_jobs.push_back(job.toString());
}
