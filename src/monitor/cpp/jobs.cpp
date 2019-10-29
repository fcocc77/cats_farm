#include "../hpp/jobs.hpp"

jobs_class::jobs_class(Ui::MainWindow *_ui)
{
    ui = _ui;


    jobs = ui->jobs;

    properties();
}

jobs_class::~jobs_class()
{
}

void jobs_class::properties()
{
    jobs->setColumnWidth(2, 200);
}