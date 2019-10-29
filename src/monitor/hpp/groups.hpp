#ifndef GROUPS_HPP
#define GROUPS_HPP

#include "../hpp/includes.hpp"

class groups_class
{
private:
    Ui::MainWindow *ui;
    QTreeWidget *groups;
    void properties();

public:
    groups_class(Ui::MainWindow *_ui);
    ~groups_class();

    
};

#endif //GROUPS_HPP