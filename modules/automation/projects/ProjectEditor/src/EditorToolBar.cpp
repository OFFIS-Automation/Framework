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

#include <QFileDialog>
#include <QMessageBox>

#include "EditorToolBar.h"
#include "ui_EditorToolBar.h"

#include "HilecSingleton.h"

EditorToolBar::EditorToolBar(QWidget *parent) :
    QToolBar(parent),
    ui(new Ui::EditorToolBar)
{
    ui->setupUi(this);
    //ui->actionRecentProjects->setEnabled(false);
}

EditorToolBar::~EditorToolBar()
{
    delete ui;
}

void EditorToolBar::initMenu(QMenu *fileMenu)
{
    if(fileMenu->actions().size() > 0)
        fileMenu->addSeparator();

    fileMenu->addAction(ui->actionOpen);
    fileMenu->addAction(ui->actionNew);
    fileMenu->addAction(ui->actionSave);
    fileMenu->addAction(ui->actionSaveAll);
    fileMenu->addAction(ui->actionClose_current_file);
    fileMenu->addAction(ui->actionClose_all_files);
    fileMenu->addSeparator();
    fileMenu->addAction(ui->actionFind);
}

void EditorToolBar::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open File"), mBaseDir, tr("Supported Files (*.pro *.ini *.py *.xml)"));

    if(fileName.isEmpty())
        return;

    emit openFileRequested(fileName);
}

void EditorToolBar::on_actionNew_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                 tr("New File"), mBaseDir,
                                 tr("Supported Files (*.pro *.ini *.py *.xml)"));
    if (fileName.isEmpty())
        return;

    if (!fileName.contains(mBaseDir))
    {
        QMessageBox msgBox;
        msgBox.setText("The file is not inside the project directory.");
        msgBox.setInformativeText("Do you really want to create it?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();
        if (ret == QMessageBox::Cancel)
            return;
    }

    emit newFileRequested(fileName);
}

void EditorToolBar::on_actionSave_triggered()
{
    emit saveFileRequested();
}

void EditorToolBar::on_actionCascade_triggered()
{
    emit cascadeRequested();
}

void EditorToolBar::on_actionTile_triggered()
{
    emit tileRequested();
}

void EditorToolBar::on_actionTab_triggered()
{
    emit tabRequested();
}

void EditorToolBar::on_actionSaveAll_triggered()
{
    emit saveAllRequested();
}

void EditorToolBar::on_actionClose_current_file_triggered()
{
    emit closeCurrentFileRequested();
}

void EditorToolBar::on_actionClose_all_files_triggered()
{
    emit closeAllFilesRequested();
}

void EditorToolBar::on_actionFind_triggered()
{
    emit findRequested();
}

void EditorToolBar::on_actionUndo_triggered()
{
    emit undoRequested();
}

void EditorToolBar::on_actionRedo_triggered()
{
    emit redoRequested();
}

void EditorToolBar::on_actionZoomIn_triggered()
{
    emit zoomInRequested();
}

void EditorToolBar::on_actionZoomOut_triggered()
{
    emit zoomOutRequested();
}

void EditorToolBar::on_actionZoomToNormal_triggered()
{
    emit zoomNormalRequested();
}

void EditorToolBar::on_actionCut_triggered()
{
    emit cutRequested();
}

void EditorToolBar::on_actionCopy_triggered()
{
    emit copyRequested();
}

void EditorToolBar::on_actionPaste_triggered()
{
    emit pasteRequested();
}

void EditorToolBar::onUndoStatusChanged(bool undoAvailable)
{
    ui->actionUndo->setEnabled(undoAvailable);
}

void EditorToolBar::onRedoStatusChanged(bool redoAvailable)
{
    ui->actionRedo->setEnabled(redoAvailable);
}

void EditorToolBar::onCutCopyStatusChanged(bool cutCopyAvailable)
{
    ui->actionCut->setEnabled(cutCopyAvailable);
    ui->actionCopy->setEnabled(cutCopyAvailable);
}
