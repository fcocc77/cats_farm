#ifndef TEXT_KNOB_H
#define TEXT_KNOB_H

#include <QWidget>

class text_knob : public QWidget
{
    Q_OBJECT
public:
    text_knob(QString label_name, bool line_edit = true);

signals:
    void changed(QString file_path);
};

#endif // TEXT_KNOB_H
