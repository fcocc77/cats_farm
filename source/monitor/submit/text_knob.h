#ifndef TEXT_KNOB_H
#define TEXT_KNOB_H

#include <QLineEdit>
#include <QTextEdit>
#include <QWidget>

class text_knob : public QWidget
{
    Q_OBJECT
public:
    QLineEdit *line_edit;
    QTextEdit *text_edit;

    text_knob(QString label_name, bool is_line_edit = true);

    inline QString get_text() const;
    inline void set_text(QString text);
    inline QLineEdit *get_line_edit() const;

signals:
    void changed(QString file_path);
};

inline QLineEdit *text_knob::get_line_edit() const
{
    return line_edit;
}

inline QString text_knob::get_text() const
{
    return line_edit ? line_edit->text() : text_edit->toPlainText();
}

inline void text_knob::set_text(QString text)
{
    line_edit ? line_edit->setText(text) : text_edit->setPlainText(text);
}

#endif // TEXT_KNOB_H
