// Author: Francisco Contreras
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#ifndef LOG_HPP
#define LOG_HPP

#include <QCodeEditor.hpp>
#include <QStringList>
#include <QWidget>

#include "../components/combo_box.h"

class log_class : public QWidget
{
private:
    void setup_ui();

    QString text;
    combo_box *servers_box;

public:
    log_class();
    ~log_class();

    kgl::QCodeEditor *code_editor;
    void update_log(QString text, QStringList servers = {});
};

#endif // LOG_HPP
