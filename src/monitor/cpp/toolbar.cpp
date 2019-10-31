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
}

toolbar_class::~toolbar_class()
{
}

void toolbar_class::property()
{
	ui->toolbar->setMovable(0);
	ui->toolbar->setIconSize(QSize(30, 30));

	ui->toolbar->addAction(jobs->job_resume_action);
	ui->toolbar->addSeparator();
	ui->toolbar->addAction(jobs->job_suspend_action);
	ui->toolbar->addSeparator();
	ui->toolbar->addAction(global->preferences_action);
	ui->toolbar->addSeparator();

}