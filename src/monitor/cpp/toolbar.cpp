#include "../hpp/toolbar.hpp"

toolbar_class::toolbar_class(
	Ui::MainWindow *_ui,
	global_class *_global,
	jobs_class *_jobs,
	update_class *_update,
	shared_variables *_shared)
{
	ui = _ui;
	global = _global;
	jobs = _jobs;
	update = _update;
	shared = _shared;
	property();
	load_zones();
	connections();
}

toolbar_class::~toolbar_class()
{
}

void toolbar_class::property()
{
	ui->toolbar->setMovable(0);
	ui->toolbar->setIconSize(QSize(30, 30));
}

void toolbar_class::connections()
{
	connect(ui->tool_settings, &QPushButton::clicked, this, [this]() {
		ui->settings->show();
	});

	connect(ui->tool_suspend, &QPushButton::clicked, this, [this]() {
		jobs->job_suspend_action->triggered();
	});

	connect(ui->tool_resume, &QPushButton::clicked, this, [this]() {
		jobs->job_resume_action->triggered();
	});

	connect(ui->tool_zone, &QComboBox::currentTextChanged, update, &update_class::zone_change);
}

void toolbar_class::load_zones()
{
	QJsonArray zones = shared->settings["hosts"].toArray();
	QString current_manager = shared->settings["current_manager"].toString();

	// agrega todas las zonas guardadas al combobox
	for (QJsonValue value : zones)
	{
		QString zone = value.toString();
		ui->tool_zone->addItem(zone);
	}
	// ---------------------------

	// check zona actual en el combobox
	ui->tool_zone->setCurrentText(current_manager);
	// ----------------------
	// establece por defecto la ultima zona guardada
	update->update(current_manager);
	// ----------------------
}