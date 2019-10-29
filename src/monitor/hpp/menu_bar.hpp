#ifndef MENU_BAR_HPP
#define MENU_BAR_HPP

#include "../hpp/includes.hpp"

class menu_bar_class
{
private:
    Ui::MainWindow *ui;

public:
    menu_bar_class(Ui::MainWindow *_ui);
    ~menu_bar_class();
};

#endif //MENU_BAR_HPP