#include "../hpp/tasks.hpp"

tasks_class::tasks_class(Ui::MainWindow *_ui)
{
    ui = _ui;


    tasks = ui->tasks;

    properties();
}

tasks_class::~tasks_class()
{
}

void tasks_class::properties()
{
    tasks->setColumnWidth(2, 200);
}