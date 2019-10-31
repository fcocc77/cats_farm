#include "../hpp/options.hpp"

options_class::options_class(
    Ui::MainWindow *_ui)
{
    ui = _ui;
    property();
}

options_class::~options_class()
{
}

void options_class::property()
{
    ui->options->hide();
}
