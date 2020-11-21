#include <log.h>

log_class::log_class()
{
    setup_ui();
}

log_class::~log_class()
{
}
void log_class::setup_ui()
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

    QVBoxLayout *hbox = new QVBoxLayout();
    hbox->addWidget(code_editor);
    this->setLayout(hbox);

    code_editor->setObjectName("Log");
}
