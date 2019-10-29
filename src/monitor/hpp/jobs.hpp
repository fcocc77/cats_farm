#ifndef JOBS_HPP
#define JOBS_HPP

#include "../hpp/includes.hpp"

class jobs_class
{
private:
    Ui::MainWindow *ui;
    QTreeWidget *jobs;
    void properties();

public:
    jobs_class(Ui::MainWindow *_ui);
    ~jobs_class();

    
};

#endif //JOBS_HPP