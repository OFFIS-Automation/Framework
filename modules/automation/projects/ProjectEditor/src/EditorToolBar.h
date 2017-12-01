// OFFIS Automation Framework
// Copyright (C) 2013-2017 OFFIS e.V.
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

#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QToolBar>
#include <QMenu>

namespace Ui {
    class EditorToolBar;
}

class EditorToolBar : public QToolBar
{
    Q_OBJECT

public:
    explicit EditorToolBar(QWidget *parent = 0);
    ~EditorToolBar();
    void initMenu(QMenu* fileMenu);
    void setBaseDir(const QString& baseDir) { mBaseDir = baseDir; }

signals:
    void newFileRequested(QString filename);
    void openFileRequested(QString filename);
    void saveFileRequested();
    void cascadeRequested();
    void tileRequested();
    void tabRequested();
    void saveAllRequested();
    void closeCurrentFileRequested();
    void closeAllFilesRequested();
    void findRequested();
    void undoRequested();
    void redoRequested();
    void cutRequested();
    void copyRequested();
    void pasteRequested();
    void zoomInRequested();
    void zoomOutRequested();
    void zoomNormalRequested();

private slots:
    void on_actionOpen_triggered();
    void on_actionNew_triggered();
    void on_actionSave_triggered();
    void on_actionCascade_triggered();
    void on_actionTile_triggered();
    void on_actionTab_triggered();
    void on_actionSaveAll_triggered();
    void on_actionClose_current_file_triggered();
    void on_actionClose_all_files_triggered();
    void on_actionFind_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionZoomIn_triggered();
    void on_actionZoomOut_triggered();
    void on_actionCut_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();

    void onUndoStatusChanged(bool undoAvailable);
    void onRedoStatusChanged(bool redoAvailable);
    void onCutCopyStatusChanged(bool cutCopyAvailable);

    void on_actionZoomToNormal_triggered();

private:
    Ui::EditorToolBar *ui;
    QString mBaseDir;
};

#endif // TOOLBAR_H
