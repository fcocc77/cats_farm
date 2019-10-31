#include "../hpp/log.hpp"

log_class::log_class(
    Ui::MainWindow *_ui)
{
    ui = _ui;
    property();
}

log_class::~log_class()
{
}

void log_class::property()
{
    ui->log->hide();
}
