#include "../hpp/global.hpp"

global_class::global_class(
	QMainWindow *_monitor,
	shared_variables *_shared,
	QDockWidget *_settings_dock,
	QWidget *_options_widget,
	QWidget *_log_widget,
	QWidget *_properties)
{
	monitor = _monitor;
	shared = _shared;
	settings_dock = _settings_dock;
	options_widget = _options_widget;
	log_widget = _log_widget;
	properties = _properties;

	// General Action
	preferences_action = new QAction("Preferences");
	quit_action = new QAction("Quit");
	hide_action = new QAction("Hide");
	show_action = new QAction("Show App");
	hide_all_panels_action = new QAction("Hide Panels");
	update_style_action = new QAction("Update Style");
	//------------------------------------------------

	notify_icon();
	connections();
	style();
}

void global_class::connections()
{
	connect(preferences_action, &QAction::triggered, this, [this]() {
		settings_dock->show();
	});

	preferences_action->setIcon(QIcon(path + "/icons/setting.png"));
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

	connect(show_action, &QAction::triggered, this, [this]() {
		monitor->show();
	});

	connect(hide_all_panels_action, &QAction::triggered, this, [this]() {
		properties->hide();
		options_widget->hide();
		log_widget->hide();
		settings_dock->hide();
	});
	hide_all_panels_action->setShortcut(QString("Esc"));

	connect(update_style_action, &QAction::triggered, this, &global_class::style);
	update_style_action->setShortcut(QString("Ctrl+R"));
}

void global_class::style()
{
	// estilo de general
	QString style = fread(path + "/src/monitor/sass/main.css");
	monitor->setStyleSheet(style.toStdString().c_str());
	// ----------------------------
}

void global_class::notify_icon()
{
	notify = new QSystemTrayIcon(QIcon(path + "/icons/icon.png"), monitor);

	connect(notify, &QSystemTrayIcon::messageClicked, this, [this]() {
		monitor->show();
	});

	connect(notify, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
		if (reason == QSystemTrayIcon::Trigger)
		{
			// al clickear el icono muestra o oculta la ventana principal
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
	for (QJsonValue job : jafs(fread(path + "/log/trayIcon")))
		completed_jobs.push_back(job.toString());
	// ----------------------------------------------
}
