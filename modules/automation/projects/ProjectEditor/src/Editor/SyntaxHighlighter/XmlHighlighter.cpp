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

#include "XmlHighlighter.h"


XMLHighlighter::XMLHighlighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent)
{
    HighlightingRule rule;
    tagFormat.setFontWeight(QFont::Bold);
    tagFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("<[A-Za-z]+>");
    rule.format = tagFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegExp("</[A-Za-z]+>");
    rule.format = tagFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegExp("<[A-Za-z]+");
    rule.format = tagFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegExp(">");
    rule.format = tagFormat;
    highlightingRules.append(rule);

    processingFormat.setFontWeight(QFont::Bold);
    processingFormat.setForeground(Qt::black);
    rule.pattern = QRegExp("<\\?");
    rule.format = processingFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegExp("\\?>");
    rule.format = processingFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.pattern.setMinimal(true);
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegExp("'.*'");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::red);

    commentStartExpression = QRegExp("<!--");
    commentEndExpression = QRegExp("-->");
}

void XMLHighlighter::highlightBlock(const QString &text)
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
    setCurrentBlockState(0);
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);
    while (startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}