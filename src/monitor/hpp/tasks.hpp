#ifndef TASKS_HPP
#define TASKS_HPP

#include "../hpp/includes.hpp"

class tasks_class
{
private:
    Ui::MainWindow *ui;
    QTreeWidget *tasks;
    void properties();

public:
    tasks_class(Ui::MainWindow *_ui);
    ~tasks_class();

    
};

#endif //TASKS_HPP