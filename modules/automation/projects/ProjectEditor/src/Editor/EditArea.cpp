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

#include <QMdiSubWindow>
#include <QFileDialog>
#include <QSettings>
#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QShortcut>

#include "EditArea.h"
#include "ui_EditArea.h"
#include "FileEditor.h"
#include "../HilecSingleton.h"

EditArea::EditArea(QWidget *parent) : QDockWidget(parent), ui(new Ui::EditArea)
{
    mForwardSearch = true;
    mFoundFirst = false;

    // Set up UI
    ui->setupUi(this);
    ui->findBox->hide();
    QSettings settings;

    bool tabbed = settings.value("ProjectEditor/tabbed", true).toBool();
    if(tabbed)
        setTabView();

    connect(this, SIGNAL(addBreakpoint(QString,int)), HilecSingleton::hilec(), SLOT(addBreakpoint(QString,int)));
    connect(this, SIGNAL(removeBreakpoint(QString,int)), HilecSingleton::hilec(), SLOT(removeBreakpoint(QString,int)));
}

EditArea::~EditArea()
{
    delete ui;
}

void EditArea::newFile(QString fileName)
{
    QFile file(fileName);
    file.open(QFile::WriteOnly);
    file.close();
    openFile(fileName);
}

void EditArea::focusLine(const QString &filename, int line)
{
    foreach (QMdiSubWindow* mdiWindow, ui->mdiArea->subWindowList())
    {
        FileEditor *editor = qobject_cast<FileEditor*>(mdiWindow->widget());
        if(editor)
            editor->focusLine(-1);
    }
    if(line == -1 || filename.isEmpty())
        return;
    openFile(filename);
    QMdiSubWindow* mdiWindow = ui->mdiArea->activeSubWindow();
    if (!mdiWindow)
        return;

    FileEditor *editor = qobject_cast<FileEditor*>(mdiWindow->widget());
    if (!editor)
        return;
    editor->focusLine(line);
}

void EditArea::openFile(QString fileName)
{
    // Check if file is already opened ...
    foreach (QMdiSubWindow* mdiWindow, ui->mdiArea->subWindowList())
    {
        if (!mdiWindow)
            continue;
        FileEditor *editor = qobject_cast<FileEditor*>(mdiWindow->widget());
        if(!editor) continue;
        if(QFileInfo(fileName) == QFileInfo(editor->filename()))
        {
            ui->mdiArea->setActiveSubWindow(mdiWindow);
            return;
        }
    }

    // not open, create
    QMdiSubWindow* mdiWindow = new QMdiSubWindow();
    FileEditor *editor = new FileEditor(fileName, mdiWindow, mBaseDir);
    mdiWindow->setWidget(editor);
    mdiWindow->setAttribute(Qt::WA_DeleteOnClose);
    ui->mdiArea->addSubWindow(mdiWindow);
    mdiWindow->show();

    editor->zoomTo(currentFontSize());
    // Check permissions of file
    editor->setReadOnly(!QFileInfo(fileName).isWritable());

    // Connect user interaction
    connect(editor, SIGNAL(saveFileRequested()), this, SLOT(saveFile()));
    connect(editor, SIGNAL(increaseFontSizeRequested()), this, SLOT(increaseFontSize()));
    connect(editor, SIGNAL(decreaseFontSizeRequested()), this, SLOT(decreaseFontSize()));
    connect(editor, SIGNAL(normalizeFontSizeRequested()), this, SLOT(setFontSize()));
    connect(editor, SIGNAL(clickedProblem(QString,int)), SIGNAL(clickedProblem(QString,int)));
    connect(editor, SIGNAL(textChanged()), this, SLOT(textChanged()));
    connect(editor, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
    connect(editor, SIGNAL(close(QString)), SLOT(closeFile(QString)));
    emit fileOpened(fileName);
}

void EditArea::saveFile()
{
    QMdiSubWindow* mdiWindow = ui->mdiArea->activeSubWindow();
    if (!mdiWindow)
        return;
    FileEditor *editor = qobject_cast<FileEditor*>(mdiWindow->widget());
    if (!editor)
        return;
    if(!editor->saveContent()){
        QMessageBox msgBox;
        msgBox.setText(tr("File Save Error"));
        msgBox.setText(tr("There was an error while saving the file %1. Maybe the file is read-only.").arg(editor->filename()));
        msgBox.exec();
    } else {
       emit fileSaved(editor->filename());
    }
}

void EditArea::saveAll()
{
    foreach (QMdiSubWindow* mdiWindow, ui->mdiArea->subWindowList())
    {
        if (!mdiWindow)
            continue;
        FileEditor *editor = qobject_cast<FileEditor*>(mdiWindow->widget());
        if (!editor)
            continue;
        if(!editor->saveContent()){
            QMessageBox msgBox;
            msgBox.setText(tr("File Save Error"));
            msgBox.setText(tr("There was an error while saving the file %1. Maybe the file is read-only.").arg(editor->filename()));
            msgBox.exec();
        } else {
           emit fileSaved(editor->filename());
        }
    }
}

void EditArea::closeAll()
{
    saveAll();
    ui->mdiArea->closeAllSubWindows();
}

void EditArea::closeCurrent()
{
    ui->mdiArea->closeActiveSubWindow();
}

void EditArea::setTabView()
{
    QSettings settings;
    settings.setValue("ProjectEditor/tabbed", true);
    ui->mdiArea->setViewMode(QMdiArea::TabbedView);
    QTabBar* tab = findChild<QTabBar*>();
    tab->setTabsClosable(true);
}

void EditArea::setSubView()
{
    QSettings settings;
    settings.setValue("ProjectEditor/tabbed", false);
    ui->mdiArea->setViewMode(QMdiArea::SubWindowView);
}

void EditArea::setBaseDir(QString baseDir)
{
    mBaseDir = baseDir;
}

void EditArea::increaseFontSize()
{
    setFontSize(currentFontSize() + 1);
}

void EditArea::decreaseFontSize()
{
    setFontSize(currentFontSize() - 1);
}

int EditArea::currentFontSize()
{
    return QSettings().value("ProjectEditor/zoom", 0).toInt();
}

void EditArea::setFontSize(int size)
{
    foreach (QMdiSubWindow* mdiWindow, ui->mdiArea->subWindowList())
    {
        if (!mdiWindow)
            continue;
        FileEditor *editor = qobject_cast<FileEditor*>(mdiWindow->widget());
        if (!editor)
            continue;
        editor->zoomTo(size);
        QSettings().setValue("ProjectEditor/zoom", size);
    }
}

void EditArea::on_mdiArea_subWindowActivated(QMdiSubWindow *mdiWindow)
{
    if(!mdiWindow)
    {
        emit currentFileChanged(QString());
        return;
    }
    FileEditor *editor = qobject_cast<FileEditor*>(mdiWindow->widget());
    if(editor)
        emit currentFileChanged(editor->filename());
}

void EditArea::closeFile(QString filename)
{
    // Check if file is opened ...
    foreach (QMdiSubWindow* mdiWindow, ui->mdiArea->subWindowList())
    {
        if (!mdiWindow)
            continue;
        FileEditor *editor = qobject_cast<FileEditor*>(mdiWindow->widget());
        if(!editor)
            continue;
        if(QFileInfo(filename) == QFileInfo(editor->filename()))
        {
            ui->mdiArea->setActiveSubWindow(mdiWindow);
            ui->mdiArea->closeActiveSubWindow();
            return;
        }
    }
}

void EditArea::renameFile(QString oldName, QString newName)
{
    // Check if file is opened ...
    foreach (QMdiSubWindow* mdiWindow, ui->mdiArea->subWindowList())
    {
        if (!mdiWindow)
            continue;
        FileEditor *editor = qobject_cast<FileEditor*>(mdiWindow->widget());
        if(!editor)
            continue;

        if(QFileInfo(oldName) == QFileInfo(editor->filename()))
        {
            editor->fileRenamed(newName);
            return;
        }
    }
    // if no editor is open, check all breakpoints
    foreach(int line, HilecSingleton::hilec()->breakpoints(oldName))
    {
        emit removeBreakpoint(oldName, line);
        emit addBreakpoint(newName, line);
    }

}

void EditArea::renameDir(QString oldName, QString newName)
{
    // Check if dir is opened ...
    QString oldPath = QFileInfo(oldName).absoluteFilePath();
    foreach (QMdiSubWindow* mdiWindow, ui->mdiArea->subWindowList()){
        if (!mdiWindow)
            continue;
        FileEditor *editor = qobject_cast<FileEditor*>(mdiWindow->widget());
        if(!editor)
            continue;
        QFileInfo fileInfo(editor->filename());
        if(fileInfo.absoluteFilePath().startsWith(oldPath)){
            QString newFullPath = fileInfo.absoluteFilePath().replace(oldPath,newName);
            editor->fileRenamed(newFullPath);
        }
    }
    typedef QPair<QString, int> BreakpointPair;
    foreach(BreakpointPair breakpoint, HilecSingleton::hilec()->breakpoints())
    {
        QFileInfo fileInfo(breakpoint.first);
        if(fileInfo.absolutePath().startsWith(oldPath)){
            QString newFullPath = fileInfo.absolutePath().replace(oldPath,newName);
            emit removeBreakpoint(breakpoint.first, breakpoint.second);
            emit addBreakpoint(newFullPath, breakpoint.second);
        }
    }
}

void EditArea::search(QString word)
{
    // Get current editor to perform search
    FileEditor* editor = currentEditor();
    if(editor)
        editor->findFirstOccurence(word, false, false, true, editor->currentLine());
}

void EditArea::replace(QString word)
{
    // Get current editor to perform replace
    FileEditor* editor = currentEditor();
    if (editor)
        editor->replace(word);
}

FileEditor *EditArea::currentEditor()
{
    if (!ui->mdiArea->activeSubWindow())
        return 0;
    return qobject_cast<FileEditor*>(ui->mdiArea->activeSubWindow()->widget());
}

bool EditArea::search(bool forwardSearch)
{
    // Get current editor to perform search
    FileEditor* editor = currentEditor();
    if (!editor)
        return false;

    if(forwardSearch != mForwardSearch){
        // Search direction has changed, find first with new direction
        mForwardSearch = forwardSearch;
        mFoundFirst = false;
    }

    // Search for word
    if(!mFoundFirst)
        mFoundFirst = editor->findFirstOccurence(ui->searchValue->text(), ui->caseSensitive->isChecked(), ui->wholeWords->isChecked(), mForwardSearch, editor->currentLine());
    else
        mFoundFirst = editor->findNextOccurence();

    return mFoundFirst;
}


void EditArea::replace()
{
    // Get current editor to perform replace
    FileEditor* editor = currentEditor();
    if (!editor)
        return;

    if(editor->hasSelectedText() && editor->selectedText().compare(ui->searchValue->text(), Qt::CaseInsensitive) == 0)
        editor->replaceSelectedText(ui->replaceValue->text());
}

void EditArea::undo()
{
    // Get current editor to perform undo
    FileEditor* editor = currentEditor();
    if (editor && editor->isUndoAvailable())
        editor->undo();
}


void EditArea::redo()
{
    // Get current editor to perform redo
    FileEditor* editor = currentEditor();
    if (editor && editor->isRedoAvailable())
        editor->redo();
}

void EditArea::cut()
{
    // Get current editor to perform redo
    FileEditor* editor = currentEditor();
    if (editor && editor->hasSelectedText())
        editor->cut();
}

void EditArea::copy()
{
    // Get current editor to perform redo
    FileEditor* editor = currentEditor();
    if (editor && editor->hasSelectedText())
        editor->copy();
}

void EditArea::paste()
{
    // Get current editor to perform redo
    FileEditor* editor = currentEditor();
    if (editor)
        editor->paste();
}

void EditArea::showFind()
{
   // Show box
   ui->findBox->show();

   // Get current selection as initial value for search
   FileEditor* editor = currentEditor();
   if(!editor)
       return;

   if(editor->hasSelectedText())
       ui->searchValue->setText(editor->selectedText());
   ui->searchValue->setFocus();
}

void EditArea::on_searchNext_clicked()
{
    search(true);
}

void EditArea::on_searchPrev_clicked()
{
    search(false);
}

void EditArea::on_replace_clicked()
{
    replace();
}

void EditArea::on_replaceNext_clicked()
{
    replace();
    search();
}

void EditArea::on_replaceAll_clicked()
{
    FileEditor* editor = currentEditor();
    if(!editor)
        return;
    while(search())
        replace();
}

void EditArea::cascade()
{
    setSubView();
    ui->mdiArea->cascadeSubWindows();
}

void EditArea::tile()
{
    setSubView();
    ui->mdiArea->tileSubWindows();
}

void EditArea::textChanged()
{
    // Get current editor to perform redo
    FileEditor* editor = currentEditor();
    if (!editor)
        return;

    // Emit status
    emit redoStatusChanged(editor->isRedoAvailable());
    emit undoStatusChanged(editor->isUndoAvailable());
}

void EditArea::selectionChanged()
{
    // Get current editor to perform redo
    FileEditor* editor = currentEditor();
    if (!editor)
        return;

    // Get status
    emit cutCopyStatusChanged(editor->hasSelectedText());
}

void EditArea::on_searchValue_textChanged(const QString &)
{
    mFoundFirst = false;
}
