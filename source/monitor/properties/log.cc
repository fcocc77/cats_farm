// Author: Francisco Jose Contreras Cuevas
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#include <QVBoxLayout>

#include "log.h"

log_class::log_class()
{
    setup_ui();
}

log_class::~log_class() {}

void log_class::setup_ui()
{
    code_editor = new kgl::QCodeEditor();

    // esto es para que el log tenga colores
    kgl::QCodeEditorDesign design(":/design.xml");
    QList<kgl::QSyntaxRule> rules =
        kgl::QSyntaxRules::loadFromFile(":/rule_cpp.xml", design);

    code_editor->setDesign(design);
    code_editor->setRules(rules);
    code_editor->setKeywords(
        {"printf", "scanf", "atoi", "mbtoa", "strlen", "memcpy", "memset"});
    code_editor->setReadOnly(true);

    QWidget *choice_widget = new QWidget;

    QVBoxLayout *vbox = new QVBoxLayout;
    QHBoxLayout *choice_layout = new QHBoxLayout(choice_widget);

    code_editor->setObjectName("Log");

    servers_box = new combo_box();
    combo_box *instances_box = new combo_box();

    QStringList instances;
    for (int i = 0; i < 16; i++)
        instances.push_back("Instance " + QString::number(i));

    instances_box->add_item("All");
    instances_box->add_items(instances);

    instances_box->set_index(0);

    // Layout
    this->setLayout(vbox);

    choice_layout->addWidget(servers_box);
    choice_layout->addWidget(instances_box);

    vbox->addWidget(choice_widget);
    vbox->addWidget(code_editor);
}

void log_class::update_log(QString _text, QStringList servers)
{
    text = _text;

    servers_box->clear();
    servers_box->add_items(servers);
    servers_box->set_index(0);

    code_editor->setPlainText(text);
}
