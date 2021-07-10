#ifndef LOG_HPP
#define LOG_HPP

#include <QWidget>
#include <QCodeEditor.hpp>

class log_class : public QWidget
{
private:
    void setup_ui();

public:
    log_class();
    ~log_class();

    kgl::QCodeEditor *code_editor;
    inline void set_text(QString text);
};

inline void log_class::set_text(QString text)
{
    code_editor->setPlainText(text);
}

#endif // LOG_HPP
