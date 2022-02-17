// Author: Francisco Jose Contreras Cuevas
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#include <QHBoxLayout>
#include <QLabel>

#include "submit_global.h"
#include "text_knob.h"

text_knob::text_knob(QString label_name, bool is_line_edit)
    : line_edit(nullptr)
    , text_edit(nullptr)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);

    label = new QLabel(label_name + ":");

    QWidget *edit;

    if (is_line_edit)
    {
        line_edit = new QLineEdit();
        edit = line_edit;
    }
    else
    {
        text_edit = new QTextEdit();
        edit = text_edit;
    }

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

void text_knob::set_disabled(bool disable)
{
    line_edit ? line_edit->setDisabled(disable)
              : text_edit->setDisabled(disable);

    label->setDisabled(disable);
}

