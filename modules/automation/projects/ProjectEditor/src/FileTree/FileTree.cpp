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

#include "FileTree.h"
#include "ui_FileTree.h"

#include <QMouseEvent>
#include <QDebug>
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QInputDialog>

FileTree::FileTree(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::FileTree)
{
    ui->setupUi(this);
    model.setReadOnly(false);
    ui->treeView->setAnimated(false);
    ui->treeView->setIndentation(20);
    ui->treeView->setSortingEnabled(true);
    ui->treeView->setWindowTitle(QObject::tr("Directory View"));
    ui->treeView->show();
    QSettings settings;
    mFileFilter = settings.value("ProjectEditor/filefilter", false).toBool();
    if (mFileFilter)
    {
        QStringList files;
        model.setNameFilterDisables(false);
        files << "*.py" << "*.xml" << "*.pro";
        model.setNameFilters(files);
    }
    else
    {
        QStringList files;
        model.setNameFilters(files);
    }
}

FileTree::~FileTree()
{
    delete ui;
}

void FileTree::updateTree(QString directoryName)
{
    mBaseDir = directoryName;
    model.setRootPath(directoryName);
    ui->treeView->setModel(&model);
    ui->treeView->setRootIndex(model.index(directoryName));
    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);
}

void FileTree::clear()
{
    ui->treeView->setModel(0);
}

void FileTree::on_treeView_doubleClicked(const QModelIndex &index)
{
    if (model.fileInfo(index).absoluteFilePath().isEmpty())
        return;
    if (model.fileInfo(index).isDir())
        return;

    emit openFileRequested(model.fileInfo(index).absoluteFilePath());
}

void FileTree::keyPressEvent(QKeyEvent *)
{

}

void FileTree::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu;
    menu.addAction(ui->newFile);
    if (!model.fileInfo(ui->treeView->currentIndex()).isDir() && ui->treeView->indexAt(pos).isValid())
        menu.addAction(ui->deleteFile);
    if (ui->treeView->indexAt(pos).isValid())
        menu.addAction(ui->renameFile);
    menu.addAction(ui->changeFilter);
    ui->changeFilter->setChecked(mFileFilter);
    menu.exec(mapToGlobal(pos));
}

void FileTree::on_newFile_triggered()
{
    QString dir = mBaseDir;
    if(ui->treeView->currentIndex().isValid())
    dir = model.fileInfo(ui->treeView->currentIndex()).absolutePath();
    QString fileName = QFileDialog::getSaveFileName(this,
                                 tr("New File"), dir,
                                 tr("Automation Files (*.py)"));
    if (fileName.isEmpty())
        return;

    if (!fileName.contains(mBaseDir))
    {
        QMessageBox msgBox;
        msgBox.setText(tr("The file is not inside the project directory."));
        msgBox.setInformativeText(tr("Do you really want to create it?"));
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();
        if (ret == QMessageBox::Cancel)
            return;
    }

    emit newFileRequested(fileName);
}

void FileTree::on_deleteFile_triggered()
{
    QMessageBox msgBox;
    msgBox.setText(tr("Delete file?"));
    msgBox.setInformativeText(tr("Do you really want to delete this file?"));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Cancel)
        return;
    model.remove(ui->treeView->currentIndex());
    emit closeFileRequested(model.fileInfo(ui->treeView->currentIndex()).absoluteFilePath());
}

void FileTree::on_renameFile_triggered()
{
    ui->treeView->edit(ui->treeView->currentIndex());
}

void FileTree::on_changeFilter_triggered()
{
    mFileFilter = !mFileFilter;
    QSettings settings;
    if (mFileFilter)
    {
        QStringList files;
        model.setNameFilterDisables(false);
        files << "*.py" << "*.xml" << "*.pro" << "*.ini";
        model.setNameFilters(files);
    }
    else
    {
        QStringList files;
        model.setNameFilters(files);
    }
    settings.setValue("ProjectEditor/filefilter", mFileFilter);
}
