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

#include "CallStackWidget.h"
#include "ui_CallStackWidget.h"

#include "../HilecSingleton.h"

#include <QFileInfo>

CallStackWidget::CallStackWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::CallStackWidget)
{
    ui->setupUi(this);
    connect(HilecSingleton::hilec(), SIGNAL(breakpointHit(QString,int)), SLOT(updateStack()));
}



CallStackWidget::~CallStackWidget()
{
    delete ui;
}

void CallStackWidget::updateStack()
{
    ui->listWidget->clear();
    QList<TraceLine> trace = HilecSingleton::hilec()->getStackTrace();
    foreach(const TraceLine& line, trace)
    {
        QString text = QString(line.file).remove(mBaseDir) + ": " + line.method + ", " + tr("line", "as in filename:test(), line 12", line.line) + QString::number(line.line);
        QListWidgetItem* item = new QListWidgetItem(text);
        item->setData(Qt::UserRole, line.file);
        item->setData(Qt::UserRole+1, line.line);
        ui->listWidget->addItem(item);
    }
}

void CallStackWidget::setProjectFile(const QString &projectFile)
{
    mBaseDir = QFileInfo(projectFile).canonicalPath();
}

void CallStackWidget::on_listWidget_itemActivated(QListWidgetItem *item)
{
    int row = ui->listWidget->row(item);
    HilecSingleton::hilec()->getDebugVars(row);
    QString file = item->data(Qt::UserRole).toString();
    int line = item->data(Qt::UserRole + 1).toInt();
    emit focusLine(file, line);
}
