// OFFIS Automation Framework
// Copyright (C) 2013-2016 OFFIS e.V.
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

#ifndef FILETREE_H
#define FILETREE_H

#include <QDockWidget>
#include <QTreeView>
#include <QToolButton>
#include <QFileSystemModel>
#include "FileModelFilterProxy.h"

namespace Ui {
    class FileTree;
}

class FileTree : public QDockWidget
{
    Q_OBJECT

public:
    explicit FileTree(QWidget *parent = 0);
    ~FileTree();

signals:
    void openFileRequested(QString filename);
    void newFileRequested(QString filename);
    void fileRenamed(QString oldName, QString newName);
    void directoryRenamed(QString oldName, QString newName);
    void closeFileRequested(QString filename);

public slots:
    void updateTree(QString directoryName);

private slots:
    void on_treeView_doubleClicked(const QModelIndex &index);
    void on_treeView_customContextMenuRequested(const QPoint &pos);
    void on_model_fileRenamed(const QString & path, const QString &oldName, const QString &newName);
    void on_newFile_triggered();
    void on_deleteFile_triggered();
    void on_renameFile_triggered();
    void on_openExplorer_triggered();

private:
    Ui::FileTree *ui;
    QFileSystemModel mModel;
    FileModelFilterProxy mFilteredModel;
    QString mBaseDir;
    bool mFileFilter;
    void keyPressEvent(QKeyEvent *e);
};

#endif // FILETREE_H
