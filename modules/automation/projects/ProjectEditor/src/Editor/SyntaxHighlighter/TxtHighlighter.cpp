// OFFIS Automation Framework
// Copyright (C) 2013 OFFIS e.V.
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "TxtHighlighter.h"

TxtHighlighter::TxtHighlighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent)
{
    HighlightingRule rule;
    QTextCharFormat tagFormat;

    tagFormat.setFontWeight(QFont::Bold);
    tagFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\\[.*\\]");
    rule.format = tagFormat;
    highlightingRules.append(rule);

    tagFormat.setFontWeight(QFont::Normal);
    tagFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp(".*=");
    rule.format = tagFormat;
    highlightingRules.append(rule);
    tagFormat.setFontWeight(QFont::Normal);
    tagFormat.setForeground(Qt::red);
    rule.pattern = QRegExp("=");
    rule.format = tagFormat;
    highlightingRules.append(rule);
}

void TxtHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
}
