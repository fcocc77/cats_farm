// Author: Francisco Jose Contreras Cuevas
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#ifndef FILE_KNOB_H
#define FILE_KNOB_H

#include <QLineEdit>
#include <QWidget>

class file_knob : public QWidget
{
    Q_OBJECT
private:
    QLineEdit *edit;

public:
    file_knob(QString label, QString button_name, bool dir_method = false);

    inline QString get_path() const;
    inline void set_path(QString path);

signals:
    void changed(QString file_path);
};

inline QString file_knob::get_path() const
{
    return edit->text();
}

inline void file_knob::set_path(QString path)
{
    edit->setText(path);
}

#endif // FILE_KNOB_H
