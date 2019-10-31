#ifndef LOG_HPP
#define LOG_HPP

#include "includes.hpp"

#include "../../external/QCodeEditor/include/KGL/Widgets/QCodeEditor.hpp"

class log_class
{
private:
    Ui::MainWindow *ui;
    void property();

    kgl::QCodeEditor *code_editor;

public:
    log_class(
        Ui::MainWindow *_ui);
    ~log_class();
};

#endif // LOG_HPP