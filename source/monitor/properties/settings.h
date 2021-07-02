#ifndef SETTING_HPP
#define SETTING_HPP

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QPlainTextEdit>

#include "shared_variables.h"
#include "text_knob.h"

class settings_class : public QWidget
{
private:
    void setup_ui();
    void connections();
    void path_read();
    void path_write();
    void ok();

    QWidget *_monitor;
    shared_variables *shared;

    text_knob *zones_edit;
    text_knob *host_edit;

    QPlainTextEdit *paths_text;
    QPlainTextEdit *maya_text;
    QPlainTextEdit *nuke_text;
    QPlainTextEdit *houdini_text;
    QPlainTextEdit *ffmpeg_text;
    QPlainTextEdit *vinacomp_text;

    QPushButton *cancel_button;
    QPushButton *apply_button;
    QPushButton *ok_button;

public:
    settings_class(QWidget *_monitor, shared_variables *_shared);
    ~settings_class();
    void update(QString host);
};

#endif // SETTING_HPP
