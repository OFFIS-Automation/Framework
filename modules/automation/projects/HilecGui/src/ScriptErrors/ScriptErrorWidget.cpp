// OFFIS Automation Framework
// Copyright (C) 2013-2014 OFFIS e.V.
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

#include "ScriptErrorWidget.h"
#include "ui_ScriptErrorWidget.h"

#include "../HilecSingleton.h"

#include <QDir>
#include <QDebug>
#include <QAbstractItemView>

ScriptErrorWidget::ScriptErrorWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ScriptErrorWidget)
{
    ui->setupUi(this);
    connect(HilecSingleton::hilec(), SIGNAL(compileError(ScriptCompileInfo)), SLOT(updateCompileError(ScriptCompileInfo)));

    mErrorIcons[ScriptCompileProblem::Error] = QIcon(":/ProjectEditor/exclamation.png");
    mErrorIcons[ScriptCompileProblem::Warning] = QIcon(":/ProjectEditor/error.png");
    mErrorIcons[ScriptCompileProblem::Info] = QIcon(":/ProjectEditor/information.png");
    mModeEnabled[ScriptCompileProblem::Info] = false;
    mModeEnabled[ScriptCompileProblem::Warning] = true;
    mModeEnabled[ScriptCompileProblem::Error] = true;
    ui->enableErrors->setChecked(mModeEnabled[ScriptCompileProblem::Error]);
    ui->enableWarnings->setChecked(mModeEnabled[ScriptCompileProblem::Warning]);
    ui->enableInfos->setChecked(mModeEnabled[ScriptCompileProblem::Info]);
}

ScriptErrorWidget::~ScriptErrorWidget()
{
    delete ui;
}

void ScriptErrorWidget::updateCompileError(const ScriptCompileInfo &info)
{
    if(mBaseDir.isEmpty())
        return;
    QString file = QDir(mBaseDir).relativeFilePath(info.file);
    if(file.startsWith("..")) return; // is not a project file

    ListWidgetItems oldItems = mItems.take(file);
    foreach(QTreeWidgetItem* item, oldItems)
    {
        QTreeWidgetItem* parent = ui->treeWidget->invisibleRootItem();
        int index = parent->indexOfChild(item);
        parent->takeChild(index);
        delete item;
    }

    mItems[file].clear();
    foreach(const ScriptCompileProblem& problem, info.problems)
    {
        QStringList values;
        QString sortString = QString("%1%2%3").arg(problem.mode).arg(file).arg(problem.line, 8, 10, QChar('0')).toUpper();
        values << problem.msg << file << QString::number(problem.line) << sortString;
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget, values);
        item->setData(0, Qt::UserRole, info.file);
        item->setData(0, Qt::UserRole+1 , problem.line);
        item->setData(0, Qt::UserRole+2 , problem.mode);
        item->setIcon(0, mErrorIcons.value(problem.mode));
        item->setHidden(!mModeEnabled.value(problem.mode));
        mItems[file].append(item);
    }
    ui->treeWidget->invisibleRootItem()->sortChildren(3, Qt::AscendingOrder);
}

void ScriptErrorWidget::selectProblem(const QString &file, int line)
{
    if(mBaseDir.isEmpty())
        return;
    QString filePath = QDir(mBaseDir).relativeFilePath(file);
    if(filePath.startsWith("..")) return; // is not a project file

    foreach(QTreeWidgetItem *item, mItems[filePath]){
        // Select each item which is from file and the same line
        if(item->data(0, Qt::UserRole+1) == line)
            item->setSelected(true);
    }
}

void ScriptErrorWidget::setProjectFile(const QString &path)
{
    clear();
    mBaseDir = QFileInfo(path).canonicalPath();
}

void ScriptErrorWidget::clear()
{
    ui->treeWidget->clear();
    mItems.clear();
    mBaseDir = QString();
}


void ScriptErrorWidget::on_treeWidget_itemActivated(QTreeWidgetItem *item, int)
{
    QString file = item->data(0, Qt::UserRole).toString();
    int line = item->data(0, Qt::UserRole +1 ).toInt();
    emit focusLine(file, line);
}

void ScriptErrorWidget::on_enableErrors_toggled(bool checked)
{
    mModeEnabled[ScriptCompileProblem::Error] = checked;
    updateItems();
}

void ScriptErrorWidget::on_enableWarnings_toggled(bool checked)
{
    mModeEnabled[ScriptCompileProblem::Warning] = checked;
    updateItems();
}

void ScriptErrorWidget::on_enableInfos_toggled(bool checked)
{
    mModeEnabled[ScriptCompileProblem::Info] = checked;
    updateItems();
}

void ScriptErrorWidget::updateItems()
{
    foreach(ListWidgetItems list, mItems.values())
    {
        foreach(QTreeWidgetItem* item, list)
        {
            int mode = item->data(0, Qt::UserRole + 2).toInt();
            item->setHidden(!mModeEnabled.value(mode));
        }
    }
}
