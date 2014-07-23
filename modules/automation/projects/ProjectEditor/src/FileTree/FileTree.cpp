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
#include <QProcess>

FileTree::FileTree(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::FileTree)
{
    ui->setupUi(this);
    mModel.setReadOnly(false);
    mFilteredModel.setDynamicSortFilter(true);
    ui->treeView->setAnimated(false);
    ui->treeView->setIndentation(20);
    ui->treeView->setSortingEnabled(false);
    ui->treeView->setWindowTitle(QObject::tr("Directory View"));
    ui->treeView->show();
    // Connect to Model to get notified when a File is renamed
    connect(&mModel, SIGNAL(fileRenamed(QString, QString, QString)), this, SLOT(onFileRenamed(QString,QString,QString)));
}

FileTree::~FileTree()
{
    delete ui;
}

void FileTree::updateTree(QString directoryName)
{
    mBaseDir = directoryName;
    mModel.setRootPath(directoryName);
    mFilteredModel.setSourceModel(&mModel);
    ui->treeView->setModel(&mFilteredModel);
    ui->treeView->setRootIndex(mFilteredModel.mapFromSource(mModel.index(directoryName)));
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
    QModelIndex realIndex = mFilteredModel.mapToSource(index);
    if (mModel.fileInfo(realIndex).absoluteFilePath().isEmpty())
        return;
    if (mModel.fileInfo(realIndex).isDir())
        return;

    emit openFileRequested(mModel.fileInfo(realIndex).absoluteFilePath());
}

void FileTree::keyPressEvent(QKeyEvent *)
{

}

void FileTree::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu;
    menu.addAction(ui->newFile);
    QModelIndex realCurrentIndex = mFilteredModel.mapToSource(ui->treeView->currentIndex());
    if (!mModel.fileInfo(realCurrentIndex).isDir() && ui->treeView->indexAt(pos).isValid())
        menu.addAction(ui->deleteFile);
    if (ui->treeView->indexAt(pos).isValid()){
        menu.addAction(ui->renameFile);
        menu.addAction(ui->openExplorer);
    }
    menu.exec(mapToGlobal(pos));
}

void FileTree::onFileRenamed(const QString &path, const QString &oldName, const QString &newName)
{
    QString newFullPath = QString("%1/%2").arg(path).arg(newName);
    QString oldFullPath = QString("%1/%2").arg(path).arg(oldName);
    if(QFileInfo(newFullPath).isDir())
        emit directoryRenamed(oldFullPath, newFullPath);
    else
        emit fileRenamed(oldFullPath, newFullPath);
}

void FileTree::on_newFile_triggered()
{
    QString dir = mBaseDir;
    if(ui->treeView->currentIndex().isValid())
    {
        QModelIndex realCurrentIndex = mFilteredModel.mapToSource(ui->treeView->currentIndex());
        QFileInfo fileInfo = mModel.fileInfo(realCurrentIndex);
        if(fileInfo.isDir())
            dir = fileInfo.absoluteFilePath();
        else
            dir = fileInfo.absolutePath();
    }
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
    QModelIndex realCurrentIndex = mFilteredModel.mapToSource(ui->treeView->currentIndex());
    mModel.remove(realCurrentIndex);
    emit closeFileRequested(mModel.fileInfo(realCurrentIndex).absoluteFilePath());
}

void FileTree::on_renameFile_triggered()
{
    ui->treeView->edit(ui->treeView->currentIndex());
}


void FileTree::on_openExplorer_triggered()
{
    QModelIndex realCurrentIndex = mFilteredModel.mapToSource(ui->treeView->currentIndex());
    QFileInfo fileInfo = mModel.fileInfo(realCurrentIndex);

    // Create params
    QString param;
    if (!QFileInfo(fileInfo.absoluteFilePath()).isDir()){
        param = QLatin1String("/select,");
    }
    param += QDir::toNativeSeparators(fileInfo.absoluteFilePath());

    // Open explorer in an own thread
    QProcess *process = new QProcess(this);
    process->startDetached("explorer.exe",  QStringList(param));
}
