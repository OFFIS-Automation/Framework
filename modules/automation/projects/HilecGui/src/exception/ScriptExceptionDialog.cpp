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

#include "ScriptExceptionDialog.h"
#include "ui_ScriptExceptionDialog.h"

#include "../HilecSingleton.h"

#include <core/ScriptException.h>

ScriptExceptionDialog::ScriptExceptionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScriptExceptionDialog)
{
    ui->setupUi(this);
    qRegisterMetaType<ScriptException>("ScriptException");
    connect(HilecSingleton::hilec(), SIGNAL(scriptException(ScriptException)), SLOT(showException(ScriptException)));
    setVisible(false);
}

ScriptExceptionDialog::~ScriptExceptionDialog()
{
    delete ui;
}

void ScriptExceptionDialog::showException(const ScriptException &e)
{
    hide();
    ui->msg->setPlainText(e.name);
    ui->trace->clear();

    foreach(const ScriptException::Trace& trace, e.trace)
    {
        QString lineText = QString(trace.file).remove(e.baseDir) + ": " + trace.method + ", " + tr("line", "as in filename:test(), line 12", trace.line) +" " + QString::number(trace.line);
        QListWidgetItem* item = new QListWidgetItem(lineText);
        item->setData(Qt::UserRole, trace.file);
        item->setData(Qt::UserRole+1, trace.line);
        ui->trace->addItem(item);
    }
    show();
    focusWidget();
}



void ScriptExceptionDialog::on_trace_itemDoubleClicked(QListWidgetItem *item)
{
    QString file = item->data(Qt::UserRole).toString();
    int line = item->data(Qt::UserRole+1).toInt();
    emit focusLine(file, line);
}

void ScriptExceptionDialog::hideEvent(QHideEvent *)
{
    emit focusLine(QString(), -1);
}
