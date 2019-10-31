#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include "includes.hpp"

class options_class
{
private:
    Ui::MainWindow *ui;

public:
    options_class(
        Ui::MainWindow *_ui);
    ~options_class();
};

#endif // OPTIONS_HPP