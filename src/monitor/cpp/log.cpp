#include "../hpp/log.hpp"

log_class::log_class(
    Ui::MainWindow *_ui)
{
    ui = _ui;
    property();
}

log_class::~log_class()
{
}

void log_class::property()
{
    code_editor = new kgl::QCodeEditor();

    // esto es para que el log tenga colores
    kgl::QCodeEditorDesign design(":/design.xml");
    QList<kgl::QSyntaxRule> rules = kgl::QSyntaxRules::loadFromFile(":/rule_cpp.xml", design);

    code_editor->setDesign(design);
    code_editor->setRules(rules);
    code_editor->setKeywords({"printf", "scanf", "atoi", "mbtoa", "strlen", "memcpy", "memset"});
    code_editor->setReadOnly(true);
    //------------------------------------------

    QWidget *widget = new QWidget();
    QVBoxLayout *hbox = new QVBoxLayout();
    hbox->addWidget(code_editor);
    widget->setLayout(hbox);

    code_editor->setObjectName("Log");
    ui->log->setWidget(widget);

    ui->log->hide();
}
