// OFFIS Automation Framework
// Copyright (C) 2013-2018 OFFIS e.V.
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

#ifndef LOGSYNTAXHIGHLIGHTER_H
#define LOGSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class LogSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit LogSyntaxHighlighter(QTextDocument *parent = 0);
    void addType(const QString& prefix, const QColor& color) { mColors[prefix.toUpper()] = color; }
protected:
    virtual void highlightBlock(const QString &text);
    QMap<QString, QColor> mColors;
    
};

#endif // LOGSYNTAXHIGHLIGHTER_H
