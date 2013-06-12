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

#include <QMdiSubWindow>
#include <QFileDialog>
#include <QSettings>
#include <QDebug>
#include <QMessageBox>
#include <QPushButton>

#include "EditArea.h"
#include "ui_EditArea.h"
#include "FileEditor.h"

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
    FileEditor *editor = new FileEditor(fileName, mdiWindow);
    mdiWindow->setWidget(editor);

    QString title = QString(fileName).remove(QString("%1/").arg(mBaseDir));
    if(!QFileInfo(fileName).isWritable())
        title.append(" (Read-Only)");

    mdiWindow->setWindowTitle(title);
    mdiWindow->setAttribute(Qt::WA_DeleteOnClose);
    ui->mdiArea->addSubWindow(mdiWindow);
    mdiWindow->show();

    // Check permissions of file
    editor->setReadOnly(!QFileInfo(fileName).isWritable());


    connect(editor, SIGNAL(saveFileRequested()), this, SLOT(saveFile()));
    connect(editor, SIGNAL(increaseFontSizeRequested()), this, SLOT(increaseFontSize()));
    connect(editor, SIGNAL(decreaseFontSizeRequested()), this, SLOT(decreaseFontSize()));
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
    connect(tab, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
}

void EditArea::setSubView()
{
    QSettings settings;
    settings.setValue("ProjectEditor/tabbed", false);
    ui->mdiArea->setViewMode(QMdiArea::SubWindowView);
}

void EditArea::closeTab(int i)
{
    QList<QMdiSubWindow*> sub = ui->mdiArea->subWindowList();
    if(sub.count() <= i)
        return;
    ui->mdiArea->setActiveSubWindow(sub.at(i));
    ui->mdiArea->closeActiveSubWindow();
}

void EditArea::setBaseDir(QString baseDir)
{
    mBaseDir = baseDir;
}

void EditArea::increaseFontSize()
{
    changeFontSize(1);
}

void EditArea::decreaseFontSize()
{
    changeFontSize(-1);
}
void EditArea::changeFontSize(int changePoints)
{
    QSettings settings;
    int size = settings.value("ProjectEditor/fontsize", 12).toInt();
    size = qBound(5, size + changePoints, 40);
    settings.setValue("ProjectEditor/fontsize", size);
    foreach (QMdiSubWindow* mdiWindow, ui->mdiArea->subWindowList())
    {
        if (!mdiWindow)
            continue;
        FileEditor *editor = qobject_cast<FileEditor*>(mdiWindow->widget());
        if (!editor)
            continue;
        editor->setFontSize(size);
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
            ui->mdiArea->setActiveSubWindow(mdiWindow);
            ui->mdiArea->closeActiveSubWindow();
            openFile(newName);
            return;
        }
    }
}

void EditArea::renameDir(QString oldName, QString newName)
{
    // Check if dir is opened ...
    // @TODO: just move the filesystem pointer
    foreach (QMdiSubWindow* mdiWindow, ui->mdiArea->subWindowList()){
        if (!mdiWindow)
            continue;
        FileEditor *editor = qobject_cast<FileEditor*>(mdiWindow->widget());
        if(!editor)
            continue;
        if(QFileInfo(oldName).absolutePath() == QFileInfo(editor->filename()).absolutePath()){
            ui->mdiArea->setActiveSubWindow(mdiWindow);
            ui->mdiArea->closeActiveSubWindow();
            continue;
        }
    }
}

void EditArea::search(QString word)
{
    if (!ui->mdiArea->activeSubWindow())
        return;

    // Get current editor to perform search
    FileEditor *editor = qobject_cast<FileEditor*>(ui->mdiArea->activeSubWindow()->widget());
    editor->findFirstOccurence(word, false, false, true, editor->currentLine());
}

void EditArea::replace(QString word)
{
    if (!ui->mdiArea->activeSubWindow())
        return;
    FileEditor *editor = qobject_cast<FileEditor*>(ui->mdiArea->activeSubWindow()->widget());
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
    if (!ui->mdiArea->activeSubWindow())
         return false;

    // Get current editor to perform search
    FileEditor* editor = currentEditor();

    if(forwardSearch != mForwardSearch){
        // Search direction has changed, find first wirh new direction
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
    if (!ui->mdiArea->activeSubWindow())
         return;

    // Get current editor to perform replace
    FileEditor* editor = currentEditor();
    if(editor->hasSelectedText() && editor->selectedText().compare(ui->searchValue->text(), Qt::CaseInsensitive) == 0)
        editor->replaceSelectedText(ui->replaceValue->text());
}

void EditArea::showFind()
{
   if (!ui->mdiArea->activeSubWindow())
        return;

   // Show box
   ui->findBox->show();

   // Get current selection as initial value for search
   FileEditor* editor = currentEditor();
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


void EditArea::on_searchValue_textChanged(const QString &arg1)
{
    mFoundFirst = false;
}
