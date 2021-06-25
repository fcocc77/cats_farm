#include <general.h>
#include "../global/global.h"

general_class::general_class(QMainWindow *_monitor, shared_variables *_shared,
                           QWidget *_properties)

    : monitor(_monitor)
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
    //------------------------------------------------

    notify_icon();
    connections();
    style();
}

void general_class::connections()
{
    preferences_action->setIcon(QIcon(VINARENDER_PATH + "/icons/setting.png"));
    preferences_action->setShortcut(QString("S"));

    connect(quit_action, &QAction::triggered, this, [this]() {
        shared->app_close = true;
        monitor->close();
    });

    connect(hide_action, &QAction::triggered, this, [this]() {
        if (monitor->isVisible())
            monitor->hide();
        else
            monitor->show();
    });
    hide_action->setShortcut(QString("Ctrl+Q"));

    connect(show_action, &QAction::triggered, this,
            [this]() { monitor->show(); });

    connect(properties_hide, &QAction::triggered, this,
            [this]() { properties->parentWidget()->hide(); });
    properties_hide->setShortcut(QString("Esc"));

    connect(update_style_action, &QAction::triggered, this,
            &general_class::style);
    update_style_action->setShortcut(QString("Ctrl+R"));
}

void general_class::style()
{
    // Estilo de general
    QString style = fread(
        "/home/pancho/Documents/develop/vinarender/resources/css/style.css");
    monitor->setStyleSheet(style.toStdString().c_str());

    // Titulo de ventana
    monitor->setWindowTitle("VinaRender Monitor");
}

void general_class::notify_icon()
{
    notify =
        new QSystemTrayIcon(QIcon(VINARENDER_PATH + "/resources/icons/icon.png"), monitor);

    connect(notify, &QSystemTrayIcon::messageClicked, this,
            [this]() { monitor->show(); });

    connect(notify, &QSystemTrayIcon::activated, this,
            [this](QSystemTrayIcon::ActivationReason reason) {
                if (reason == QSystemTrayIcon::Trigger)
                {
                    // al clickear el icono muestra o oculta la ventana
                    // principal
                    if (monitor->isVisible())
                        monitor->hide();
                    else
                        monitor->show();
                    //------------------------------------------------
                }
            });
    notify->show();

    QMenu *menu = new QMenu(monitor);
    menu->addAction(show_action);
    menu->addAction(quit_action);

    notify->setContextMenu(menu);

    // Importa lista de jobs completados
    for (QJsonValue job : jafs(fread(VINARENDER_PATH + "/log/trayIcon")))
        completed_jobs.push_back(job.toString());
    // ----------------------------------------------
}
