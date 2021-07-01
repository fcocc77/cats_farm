#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>

#include "submit_global.h"
#include "text_knob.h"

text_knob::text_knob(QString label_name, bool line_edit)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);

    QLabel *label = new QLabel(label_name + ":");

    QWidget *edit;

    if (line_edit)
        edit = new QLineEdit();
    else
        edit = new QTextEdit();

    label->setFixedWidth(INIT_LABEL_WIDTH);

    if (line_edit)
        label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    else
    {
        label->setAlignment(Qt::AlignRight | Qt::AlignTop);
        label->setContentsMargins(0, 10, 0, 0);
    }

    // Layout
    layout->addWidget(label);
    layout->addWidget(edit);
}
