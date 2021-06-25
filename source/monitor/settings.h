#ifndef SETTING_HPP
#define SETTING_HPP

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QPlainTextEdit>

#include "shared_variables.h"

class settings_class : public QWidget
{
private:
    void setup_ui();
    void connections();
    void path_read();
    void path_write();
    void ok();

    shared_variables *shared;

    QLineEdit *zones_edit;
    QLineEdit *host_edit;

    QPlainTextEdit *paths_text;
    QPlainTextEdit *maya_text;
    QPlainTextEdit *nuke_text;
    QPlainTextEdit *houdini_text;
    QPlainTextEdit *vinacomp_text;

    QPushButton *cancel_button;
    QPushButton *apply_button;
    QPushButton *ok_button;

public:
    settings_class(shared_variables *_shared);
    ~settings_class();
    void update(QString host);
};

#endif // SETTING_HPP
