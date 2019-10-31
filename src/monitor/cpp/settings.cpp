#include "../hpp/settings.hpp"

settings_class::settings_class(
	Ui::MainWindow *_ui)
{
	ui = _ui;
	property();
}

settings_class::~settings_class()
{
}

void settings_class::property()
{

	ui->settings->hide();

	// ui->settings->setFloating(true);
}