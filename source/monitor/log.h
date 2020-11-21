#ifndef LOG_HPP
#define LOG_HPP

#include <includes.h>

#include <QCodeEditor.hpp>

class log_class : public QWidget
{
private:
    void setup_ui();

public:
    log_class();
    ~log_class();

    kgl::QCodeEditor *code_editor;
};

#endif // LOG_HPP