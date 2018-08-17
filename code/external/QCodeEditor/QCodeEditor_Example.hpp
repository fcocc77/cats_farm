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

#ifndef QCODEEDITOR_EXAMPLE_HPP
#define QCODEEDITOR_EXAMPLE_HPP

//
//  Included headers
//
#include <KGL/Widgets/QCodeEditor.hpp>
#include <KGL/Widgets/QCodeEditorHighlighter.hpp>
#include <QWidget>
#include <QMap>
#include <QMainWindow>
#include <QVBoxLayout>

namespace kgl {

    ///
    ///  @file      QCodeEditor_Example.hpp
    ///  @author    Nicolas Kogler
    ///  @date      October 15th, 2016
    ///  @class     QCodeEditor_Example
    ///  @brief     Form containing the QCodeEditor.
    ///
    class QCodeEditor_Example : public QWidget {
    Q_OBJECT
    public:

        ///
        ///  @fn    Default constructor
        ///  @brief Initializes a new instance of QCodeEditor_Example.
        ///
        QCodeEditor_Example(QWidget *parent = 0);

        ///
        ///  @fn    Destructor
        ///  @brief Frees resources used by QCodeEditor_Example.
        ///
        ~QCodeEditor_Example();

        QCodeEditor *m_Editor;

    private slots:

        ///
        ///  @fn    addMacro
        ///  @brief Adds the given macro.
        ///
        void addMacro(const QSyntaxRule &rule, QString seq, QTextBlock block);

        ///
        ///  @fn    removeMacro2
        ///  @brief Removes a macro under certain conditions.
        ///
        void removeMacro(QCodeEditorBlockData *data);

    private:

        //
        //  Class members
        //
        QMap<QTextBlock, QSyntaxRule> m_RuleMap;
        QMap<QTextBlock, QString> m_MacroMap;

    };
}

#endif // QCODEEDITOR_EXAMPLE_HPP
