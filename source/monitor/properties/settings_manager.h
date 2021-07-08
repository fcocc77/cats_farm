#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

#include <QWidget>
#include <QPlainTextEdit>

#include "shared_variables.h"

class settings_manager : public QWidget
{
private:
    shared_variables *shared;

    QPlainTextEdit *paths_text;
    QPlainTextEdit *maya_text;
    QPlainTextEdit *nuke_text;
    QPlainTextEdit *houdini_text;
    QPlainTextEdit *ffmpeg_text;
    QPlainTextEdit *vinacomp_text;

    void path_read();
    void path_write();


public:
    settings_manager(shared_variables *shared);

    void update(QString host);
    void save();
};
#endif // SETTINGS_MANAGER_H
