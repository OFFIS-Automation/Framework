// OFFIS Automation Framework
// Copyright (C) 2013-2018 OFFIS e.V.
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

#ifndef EDITAREA_H
#define EDITAREA_H

#include <QDockWidget>
#include <QMdiArea>
#include "FileEditor.h"


class QPlainTextEdit;

namespace Ui {
    class EditArea;
}

class EditArea : public QDockWidget
{
    Q_OBJECT

public:
    explicit EditArea(QWidget *parent = 0);
    ~EditArea();
    void setBaseDir(QString baseDir);

signals:
    void currentFileChanged(const QString& filename);
    void fileSaved(const QString& filename);
    void fileOpened(const QString& filename);
    void clickedProblem(const QString& file, int line);
    void undoStatusChanged(bool undoAvailable);
    void redoStatusChanged(bool redoAvailable);
    void cutCopyStatusChanged(bool cutCopyAvailable);
    void addBreakpoint(const QString& filename, int line);
    void removeBreakpoint(const QString& filename, int line);

public slots:
    void openFile(QString fileName);
    void saveFile();
    void saveAll();
    void closeAll();
    void closeCurrent();
    void newFile(QString fileName);
    void renameFile(QString oldName, QString newName);
    void renameDir(QString oldName, QString newName);
    void setTabView();
    void setSubView();
    void focusLine(const QString& filename, int line);
    void closeFile(QString filename);
    void search(QString word);
    void replace(QString word);
    void showFind();
    void cascade();
    void tile();
    void textChanged();
    void selectionChanged();

private slots:
    void increaseFontSize();
    void decreaseFontSize();
    void setFontSize(int size = 0);
    bool search(bool forwardSearch = false);
    void replace();
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();

    void on_searchNext_clicked();
    void on_searchPrev_clicked();
    void on_replace_clicked();

    void on_replaceNext_clicked();
    void on_replaceAll_clicked();
    void on_mdiArea_subWindowActivated(QMdiSubWindow *arg1);
    void on_searchValue_textChanged(const QString &arg1);

private:
    int currentFontSize();
    FileEditor* currentEditor();
    Ui::EditArea *ui;
    QString mBaseDir;

    bool mForwardSearch;
    bool mFoundFirst;
};

#endif // EDITAREA_H
