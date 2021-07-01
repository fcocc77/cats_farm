#ifndef FILE_KNOB_H
#define FILE_KNOB_H

#include <QWidget>

class file_knob : public QWidget
{
    Q_OBJECT
public:
    file_knob(QString label, QString button_name, bool dir_method = false);

signals:
    void changed(QString file_path);
};

#endif // FILE_KNOB_H
