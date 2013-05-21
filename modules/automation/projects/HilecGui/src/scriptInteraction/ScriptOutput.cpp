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

#include "ScriptOutput.h"
#include "ui_ScriptOutput.h"
#include "../HilecSingleton.h"


ScriptOutput::ScriptOutput(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ScriptOutput)
{
    ui->setupUi(this);
    connect(HilecSingleton::hilec(), SIGNAL(newTextStr(QString)), this, SLOT(logText(QString)), Qt::QueuedConnection);
    connect(HilecSingleton::hilec(), SIGNAL(newErrorStr(QString)), this, SLOT(logError(QString)), Qt::QueuedConnection);
    errorFormat.setForeground(QColor(Qt::red));
}

ScriptOutput::~ScriptOutput()
{
    delete ui;
}

void ScriptOutput::logText(const QString &text)
{
    log(text, textFormat);
}

void ScriptOutput::logError(const QString &text)
{
    log(text, errorFormat);
}

void ScriptOutput::log(const QString &text, const QTextCharFormat &format)
{
    QTextCursor c = ui->output->textCursor();
    bool scrollToEnd = c.atEnd();
    c.movePosition(QTextCursor::End);
    c.insertText(text, format);
    if(scrollToEnd)
    {
        c.movePosition(QTextCursor::End);
        ui->output->setTextCursor(c);
    }
}
