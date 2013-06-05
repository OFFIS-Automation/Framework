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
    mModel.setReadOnly(false);
    ui->treeView->setAnimated(false);
    ui->treeView->setIndentation(20);
    ui->treeView->setSortingEnabled(true);
    ui->treeView->setWindowTitle(QObject::tr("Directory View"));
    ui->treeView->show();
    QSettings settings;
    mFileFilter = settings.value("ProjectEditor/filefilter", false).toBool();
    if (mFileFilter){
        QStringList files;
        mModel.setNameFilterDisables(false);
        files << "*.py" << "*.xml" << "*.pro";
        mModel.setNameFilters(files);
    } else {
        QStringList files;
        mModel.setNameFilters(files);
    }

    // Connect to Model to get notified when a File is renamed
    connect(&mModel, SIGNAL(fileRenamed(QString, QString, QString)), this, SLOT(on_model_fileRenamed(QString,QString,QString)));
}

FileTree::~FileTree()
{
    delete ui;
}

void FileTree::updateTree(QString directoryName)
{
    mBaseDir = directoryName;
    mModel.setRootPath(directoryName);
    ui->treeView->setModel(&mModel);
    ui->treeView->setRootIndex(mModel.index(directoryName));
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
    if (mModel.fileInfo(index).absoluteFilePath().isEmpty())
        return;
    if (mModel.fileInfo(index).isDir())
        return;

    emit openFileRequested(mModel.fileInfo(index).absoluteFilePath());
}

void FileTree::keyPressEvent(QKeyEvent *)
{

}

void FileTree::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu;
    menu.addAction(ui->newFile);
    if (!mModel.fileInfo(ui->treeView->currentIndex()).isDir() && ui->treeView->indexAt(pos).isValid())
        menu.addAction(ui->deleteFile);
    if (ui->treeView->indexAt(pos).isValid())
        menu.addAction(ui->renameFile);
    menu.addAction(ui->changeFilter);
    ui->changeFilter->setChecked(mFileFilter);
    menu.exec(mapToGlobal(pos));
}

void FileTree::on_model_fileRenamed(const QString &path, const QString &oldName, const QString &newName)
{
    emit renameFileRequested(QString("%1/%2").arg(path).arg(oldName), QString("%1/%2").arg(path).arg(newName));
}

void FileTree::on_newFile_triggered()
{
    QString dir = mBaseDir;
    if(ui->treeView->currentIndex().isValid())
    dir = mModel.fileInfo(ui->treeView->currentIndex()).absolutePath();
    QString fileName = QFileDialog::getSaveFileName(this,
                                 tr("New File"), dir,
                                 tr("Automation Files (*.py)"));
    if (fileName.isEmpty())
        return;

    if (!fileName.contains(mBaseDir))
    {
        QMessageBox msgBox;
        msgBox.setText(tr("The File is not inside the Project Directory."));
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
    msgBox.setText(tr("Delete File?"));
    msgBox.setInformativeText(tr("Do you really want to delete this File?"));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Cancel)
        return;
    mModel.remove(ui->treeView->currentIndex());
    emit closeFileRequested(mModel.fileInfo(ui->treeView->currentIndex()).absoluteFilePath());
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
        mModel.setNameFilterDisables(false);
        files << "*.py" << "*.xml" << "*.pro" << "*.ini";
        mModel.setNameFilters(files);
    }
    else
    {
        QStringList files;
        mModel.setNameFilters(files);
    }
    settings.setValue("ProjectEditor/filefilter", mFileFilter);
}
