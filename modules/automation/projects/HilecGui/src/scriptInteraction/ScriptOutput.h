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

#ifndef SCRIPTOUTPUT_H
#define SCRIPTOUTPUT_H

#include <QDockWidget>
#include <QTextCharFormat>

namespace Ui {
    class ScriptOutput;
}

class ScriptOutput : public QDockWidget
{
    Q_OBJECT

public:
    explicit ScriptOutput(QWidget *parent = 0);
    ~ScriptOutput();
public slots:
    void logText(const QString& text);
    void logError(const QString& text);
    void log(const QString& text, const QTextCharFormat& format);
private:
    Ui::ScriptOutput *ui;
    QTextCharFormat textFormat, errorFormat;
};

#endif // SCRIPTOUTPUT_H
