#include "../hpp/toolbar.hpp"

toolbar_class::toolbar_class(
	Ui::MainWindow *_ui,
	global_class *_global,
	jobs_class *_jobs)
{
	ui = _ui;
	global = _global;
	jobs = _jobs;
	property();
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

}