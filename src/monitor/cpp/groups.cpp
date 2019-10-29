#include "../hpp/groups.hpp"

groups_class::groups_class(Ui::MainWindow *_ui)
{
    ui = _ui;


    groups = ui->groups;

    properties();
}

groups_class::~groups_class()
{
}

void groups_class::properties()
{
    groups->setColumnWidth(1, 200);
}