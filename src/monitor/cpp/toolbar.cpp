#include "../hpp/toolbar.hpp"

toolbar_class::toolbar_class(
	Ui::MainWindow *_ui,
	global_class *_global,
	jobs_class *_jobs,
	update_class *_update)
{
	ui = _ui;
	global = _global;
	jobs = _jobs;
	update = _update;
	property();
	connections();
	load_zones();
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
	QStringList zones = fread(path + "/etc/manager_host").split(",");
	// agrega todas las zonas guardadas al combobox
	// for (QString zone : zones)
	// ui->tool_zone->clear();
	// ui->tool_zone->addItem("zone");
	// ---------------------------
	// establece por defecto la primera zona
	update->update(zones[0]);
	// ----------------------
}