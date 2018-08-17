//
//  QCodeEditor - Widget to highlight and auto-complete code.
//  Copyright (C) 2016 Nicolas Kogler (kogler.cml@hotmail.com)
//
//  This file is part of QCodeEditor.
//
//  QCodeEditor is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with QCodeEditor.  If not, see <http://www.gnu.org/licenses/>.
//
//

//
//  Included headers
//
#include "QCodeEditor_Example.hpp"

namespace kgl {

    QCodeEditor_Example::QCodeEditor_Example(QWidget *parent)
        : QWidget(parent) {

        // Loads the highlighting rules from a resource file
        QCodeEditorDesign design(":/design.xml");
        QList<QSyntaxRule> rules = QSyntaxRules::loadFromFile(":/rule_cpp.xml", design);

        // Creates a new code editor, sets the rules and design, adds it to the form
        m_Editor = new QCodeEditor(this);
        m_Editor->setDesign(design);
        m_Editor->setRules(rules);
        m_Editor->setKeywords({ "printf", "scanf", "atoi", "mbtoa", "strlen", "memcpy", "memset" });

        QVBoxLayout *hbox = new QVBoxLayout();
        hbox->addWidget(m_Editor);
        this->setLayout( hbox );

        // Connects the highlighter's onMatch signal to our printMatch slot
        connect(m_Editor->highlighter(), SIGNAL(onMatch(QSyntaxRule,QString,QTextBlock)), this, SLOT(addMacro(QSyntaxRule,QString,QTextBlock)));
        connect(m_Editor->highlighter(), SIGNAL(onRemove(QCodeEditorBlockData*)), this, SLOT(removeMacro(QCodeEditorBlockData*)));
    }

    QCodeEditor_Example::~QCodeEditor_Example() {
    }

    void QCodeEditor_Example::addMacro(const QSyntaxRule &rule, QString seq, QTextBlock block) {
        // Add the macro, if not already existing
        if (rule.id() == "macro") {
            foreach (const QTextBlock &b, m_RuleMap.keys()) {
                if (b.userData() != NULL && block.userData() != NULL) {
                    auto *d1 = static_cast<QCodeEditorBlockData *>(block.userData());
                    auto *d2 = static_cast<QCodeEditorBlockData *>(b.userData());
                    if (d1->id == d2->id) {
                        return;
                    }
                }
            }

            QString def = seq.split(' ').at(0);
            m_RuleMap.insert(block, rule);
            m_MacroMap.insert(block, def);
            m_Editor->addKeyword(def);

            // TODO: Add highlighting rule for the new macro
        }
    }

    void QCodeEditor_Example::removeMacro(QCodeEditorBlockData *data) {
        foreach (const QTextBlock &b, m_RuleMap.keys()) {
            if (b.userData()) {
                auto *d = static_cast<QCodeEditorBlockData *>(b.userData());
                if (d->id == data->id) {
                    m_Editor->removeKeyword(m_MacroMap.value(b));
                    m_RuleMap.remove(b);
                    m_MacroMap.remove(b);
                }
            }
        }
    }
}
