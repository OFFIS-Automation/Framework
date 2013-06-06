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
#include "CodeEditor.h"

#define QSCINTILLA_DLL
#include <Qsci/qsciscintilla.h>

EditArea::EditArea(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::EditArea)
{
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
        CodeEditor *codeEditor = qobject_cast<CodeEditor*>(mdiWindow->widget());
        if(codeEditor) codeEditor->focusLine(-1);
    }
    if(line == -1 || filename.isEmpty())
        return;
    openFile(filename);
    QMdiSubWindow* mdiWindow = ui->mdiArea->activeSubWindow();
    if (!mdiWindow)
        return;

    CodeEditor *codeEditor = qobject_cast<CodeEditor*>(mdiWindow->widget());
    if (!codeEditor)
        return;
    codeEditor->focusLine(line);
}

void EditArea::openFile(QString fileName)
{
    // Check if file is already opened ...
    foreach (QMdiSubWindow* mdiWindow, ui->mdiArea->subWindowList())
    {
        if (!mdiWindow)
            continue;
        CodeEditor *codeEditor = qobject_cast<CodeEditor*>(mdiWindow->widget());
        if(!codeEditor) continue;
        if(QFileInfo(fileName) == QFileInfo(codeEditor->filename()))
        {
            ui->mdiArea->setActiveSubWindow(mdiWindow);
            return;
        }
    }
    // not open, create
    QMdiSubWindow* mdiWindow = new QMdiSubWindow();
    QsciScintilla *textEdit = new QsciScintilla;

    CodeEditor *codeEditor = new CodeEditor(QFileInfo(fileName).absoluteFilePath(), mdiWindow);
    mdiWindow->setWidget(codeEditor);

    QString title = QString(fileName).remove(mBaseDir).remove("/");
    if(!QFileInfo(fileName).isWritable())
        title.append(" (Read-Only)");

    mdiWindow->setWindowTitle(title);
    mdiWindow->setAttribute(Qt::WA_DeleteOnClose);
    ui->mdiArea->addSubWindow(mdiWindow);
    mdiWindow->show();

    // Check permissions of file
    codeEditor->setReadOnly(!QFileInfo(fileName).isWritable());

    connect(codeEditor, SIGNAL(saveFileRequested()), this, SLOT(saveFile()));
    connect(codeEditor, SIGNAL(increaseFontSizeRequested()), this, SLOT(increaseFontSize()));
    connect(codeEditor, SIGNAL(decreaseFontSizeRequested()), this, SLOT(decreaseFontSize()));
    emit fileOpened(codeEditor->filename());
}

void EditArea::saveFile()
{
    QMdiSubWindow* mdiWindow = ui->mdiArea->activeSubWindow();
    if (!mdiWindow)
        return;
    CodeEditor *codeEditor = qobject_cast<CodeEditor*>(mdiWindow->widget());
    if (!codeEditor)
        return;
    if(!codeEditor->save()){
        QMessageBox msgBox;
        msgBox.setText(tr("File Save Error"));
        msgBox.setText(tr("There was an error while saving the File %1. Maybe the File is Read-Only.").arg(codeEditor->filename()));
        msgBox.exec();
    } else {
       emit fileSaved(codeEditor->filename());
    }
}

void EditArea::saveAll()
{
    foreach (QMdiSubWindow* mdiWindow, ui->mdiArea->subWindowList())
    {
        if (!mdiWindow)
            continue;
        CodeEditor *codeEditor = qobject_cast<CodeEditor*>(mdiWindow->widget());
        if (!codeEditor)
            continue;
        if(!codeEditor->save()){
            QMessageBox msgBox;
            msgBox.setText(tr("File Save Error"));
            msgBox.setText(tr("There was an error while saving the File %1. Maybe the File is Read-Only.").arg(codeEditor->filename()));
            msgBox.exec();
        } else {
           emit fileSaved(codeEditor->filename());
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
        CodeEditor *codeEditor = qobject_cast<CodeEditor*>(mdiWindow->widget());
        if (!codeEditor)
            continue;
        codeEditor->setFontSize(size);
    }
}

void EditArea::on_mdiArea_subWindowActivated(QMdiSubWindow *mdiWindow)
{
    if(!mdiWindow)
    {
        emit currentFileChanged(QString());
        return;
    }
    CodeEditor *codeEditor = qobject_cast<CodeEditor*>(mdiWindow->widget());
    if(codeEditor)
        emit currentFileChanged(codeEditor->filename());
}

void EditArea::closeFile(QString filename)
{
    // Check if file is opened ...
    foreach (QMdiSubWindow* mdiWindow, ui->mdiArea->subWindowList())
    {
        if (!mdiWindow)
            continue;
        CodeEditor *codeEditor = qobject_cast<CodeEditor*>(mdiWindow->widget());
        if(!codeEditor) continue;
        if(QFileInfo(filename) == QFileInfo(codeEditor->filename()))
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
        CodeEditor *codeEditor = qobject_cast<CodeEditor*>(mdiWindow->widget());
        if(!codeEditor)
            continue;

        if(QFileInfo(oldName) == QFileInfo(codeEditor->filename()))
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
    foreach (QMdiSubWindow* mdiWindow, ui->mdiArea->subWindowList())
    {
        if (!mdiWindow)
            continue;
        CodeEditor *codeEditor = qobject_cast<CodeEditor*>(mdiWindow->widget());
        if(!codeEditor) continue;
        if(QFileInfo(oldName).absolutePath() == QFileInfo(codeEditor->filename()).absolutePath())
        {
            ui->mdiArea->setActiveSubWindow(mdiWindow);
            ui->mdiArea->closeActiveSubWindow();
            continue;
        }
    }
}

void EditArea::search(QString word, int number)
{
    if (!ui->mdiArea->activeSubWindow())
        return;
    CodeEditor *codeEditor = qobject_cast<CodeEditor*>(ui->mdiArea->activeSubWindow()->widget());
    codeEditor->search(word, number);
}

void EditArea::replace(QString word)
{
    if (!ui->mdiArea->activeSubWindow())
        return;
    CodeEditor *codeEditor = qobject_cast<CodeEditor*>(ui->mdiArea->activeSubWindow()->widget());
    codeEditor->replace(word);
}

QPlainTextEdit* EditArea::currentEditor()
{
    if (!ui->mdiArea->activeSubWindow())
        return 0;
    return qobject_cast<QPlainTextEdit*>(ui->mdiArea->activeSubWindow()->widget());
}

bool EditArea::search(bool back)
{
    QTextDocument::FindFlags flags = 0;
    if(back)
        flags |= QTextDocument::FindBackward;
    if(ui->caseSensitive->isChecked())
        flags |= QTextDocument::FindCaseSensitively;
    if(ui->wholeWords->isChecked())
        flags |= QTextDocument::FindWholeWords;
    QPlainTextEdit* edit = currentEditor();
    if(!edit) return false;
    QTextCursor c = edit->document()->find(ui->searchValue->text(), edit->textCursor(), flags);
    if(!c.isNull())
    {
        edit->setTextCursor(c);
        return true;
    }
    return false;
}


void EditArea::replace()
{
    QPlainTextEdit* edit = currentEditor();
    if(!edit) return;
    QTextCursor current = edit->textCursor();
    if(current.selectedText().compare(ui->searchValue->text(), Qt::CaseInsensitive) != 0)
        return;
    current.insertText(ui->replaceValue->text());
    edit->setTextCursor(current);
}

void EditArea::showFind()
{
    if (!ui->mdiArea->activeSubWindow())
        return;

    ui->findBox->show();
    QPlainTextEdit* edit = currentEditor();

    if(edit)
    {
        QString current = edit->textCursor().selectedText();
        if(!current.isEmpty())
            ui->searchValue->setText(current);

    }
    ui->searchValue->setFocus();
}

void EditArea::on_searchNext_clicked()
{
    search();
}

void EditArea::on_searchPrev_clicked()
{
    search(true);
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
    QPlainTextEdit* edit = currentEditor();
    if(!edit) return;
    edit->setTextCursor(QTextCursor());
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

