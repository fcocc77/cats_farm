// Author: Francisco Contreras
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "file_knob.h"
#include "submit_global.h"

file_knob::file_knob(QString label_name, QString button_name, bool dir_method)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);

    QLabel *label = new QLabel(label_name + ":");
    edit = new QLineEdit();
    QPushButton *button = new QPushButton(button_name);

    label->setFixedWidth(INIT_LABEL_WIDTH);
    label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    button->setMinimumWidth(70);

    // Conecciones
    connect(button, &QPushButton::clicked, this, [=]() {
        if (dir_method)
        {
            QString dirname = QFileDialog::getExistingDirectory(
                this, label_name, edit->text());

            if (dirname.isEmpty())
                return;

            edit->setText(dirname);
            changed(dirname);
        }
        else
        {
            QString filename =
                QFileDialog::getOpenFileName(this, label_name, edit->text());

            if (filename.isEmpty())
                return;

            edit->setText(filename);
            changed(filename);
        }
    });

    // Layout
    layout->addWidget(label);
    layout->addWidget(edit);
    layout->addWidget(button);
}
