#ifndef SERVERS_HPP
#define SERVERS_HPP

#include "../hpp/includes.hpp"

class servers_class
{
private:
    Ui::MainWindow *ui;
    QTreeWidget *servers;
    void properties();

public:
    servers_class(Ui::MainWindow *_ui);
    ~servers_class();

    
};

#endif //SERVERS_HPP