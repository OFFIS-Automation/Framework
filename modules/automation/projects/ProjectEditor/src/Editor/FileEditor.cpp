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

#include "FileEditor.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QFont>
#include <QFontMetrics>
#include <QShortcut>
#include <QMessageBox>
#include <QCoreApplication>
#include <QKeySequence>
#include <QToolTip>
#include <QMenu>
#include "qevent.h"

#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qscilexerxml.h>
#include <Qsci/qsciapis.h>

#include "../HilecSingleton.h"

#define BREAKPOINT_MARGIN 0
#define LINENUMBER_MARGIN 1
#define INFO_MARGIN 2
#define BREAKPOINTMARKER_NUMBER 1
#define CURRENT_BREAKPOINTMARKER_NUMBER 2
#define ERRORMARKER_NUMBER 3
#define EXCLAMATIONMARKER_NUMBER 4
#define INFORMATIONMARKER_NUMBER 5

typedef QPair<QString, int> BreakPointPair;

FileEditor::FileEditor(const QString &filename, QMdiSubWindow *parent, const QString &baseDir) :
    mFilename(filename), mdiWindow(parent), mBaseDir(baseDir)
{
    mFocusRecursion = mChanged = mOmitReloadChecks = false;
    mCurrentBreakpoint = -1;

    // Update GUI and content
    reloadContent();
    setupEditor();
    updateLexer();
    updateWindowTitle();
    HilecInterface* hilec = HilecSingleton::hilec();
    if(hilec)
    {
        QList<int> breakpoints = hilec->breakpoints(mFilename);
        foreach(int line, breakpoints){
            toggleBreakpoint(line-1);
        }
    }

    // Connections
    connect(this, SIGNAL(checkReloadContent()), SLOT(on_check_reload_content()), Qt::QueuedConnection);
    connect(this, SIGNAL(textChanged()), SLOT(on_text_changed()));
    connect(this, SIGNAL(asyncRemoveChangeFlag()), SLOT(removeChangedFlag()), Qt::QueuedConnection);

    // Breakpoint
    connect(this, SIGNAL(marginClicked(int,int,Qt::KeyboardModifiers)), SLOT(on_margin_clicked(int,int,Qt::KeyboardModifiers)));
    connect(this, SIGNAL(addBreakpoint(QString,int)), HilecSingleton::hilec(), SLOT(addBreakpoint(QString,int)));
    connect(this, SIGNAL(removeBreakpoint(QString,int)), HilecSingleton::hilec(), SLOT(removeBreakpoint(QString,int)));

    connect(this, SIGNAL(linesChanged()), SLOT(updateMargins()));

    // Compile error
    connect(HilecSingleton::hilec(), SIGNAL(compileError(ScriptCompileInfo)), SLOT(on_check_compile_error(ScriptCompileInfo)));

    // Keyboard Shortcurts
    new QShortcut(Qt::Key_F9, this, SLOT(toggleBreakpoint()));
}

void FileEditor::fileRenamed(const QString &newName)
{
    foreach(int key, mBreakpointMarkers.keys())
    {
        emit removeBreakpoint(mFilename, mBreakpointMarkers[key]+1);
        emit addBreakpoint(newName, mBreakpointMarkers[key]+1);
    }
    mFilename = newName;
    updateWindowTitle();
    mLastModified = QFileInfo(filename()).lastModified();
    mOmitReloadChecks = false;
}

bool FileEditor::saveContent()
{
    QFile file(filename());
    bool fileOpenSuccessful = file.open(QFile::WriteOnly  | QIODevice::Text);
    if(!fileOpenSuccessful)
        return false;

    // Save current content streambased
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream.setGenerateByteOrderMark(false);
    stream << text().replace("\r\n", "\n"); // LineMode brings no differnce
    file.close();
    mLastModified = QFileInfo(filename()).lastModified();
    mOmitReloadChecks = false;
    emit asyncRemoveChangeFlag();
    checkBreakpoints();
    return true;
}

void FileEditor::reloadContent()
{
    mLastModified = QFileInfo(filename()).lastModified();
    // Open file and display content
    QFile file(filename());
    file.open(QFile::ReadOnly);
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    setText(stream.readAll());
    file.close();

    // Remove eventually visible change flag
    removeChangedFlag();
}

void FileEditor::focusLine(int line)
{
    if(line < 1){
        // Line is invalid, remove focus
        mCurrentBreakpoint = -1;
        markerDeleteAll(CURRENT_BREAKPOINTMARKER_NUMBER);
    } else {
        // Add marker
        mCurrentBreakpoint = line;
        markerAdd(line-1, CURRENT_BREAKPOINTMARKER_NUMBER);
        // Set focus of cursor
        setCursorPosition(line-1, 0);
        // Unfold line if it is folded
        ensureLineVisible(line-1);
   }
}

void FileEditor::setFontSize(int fontSize)
{
    zoomTo(fontSize);
}

void FileEditor::removeChangedFlag()
{
    if (!mdiWindow)
        return;

    // Reset title and flag
    if (mChanged){
        mChanged = false;
        updateWindowTitle();
    }
}

void FileEditor::commentSelection()
{
    if(filename().contains(".py")){
       prependStringToSelection("#");
    }
}

void FileEditor::uncommentSelection()
{
    if(filename().contains(".py")){
       deleteStringInSelection("#");
    }
}

int FileEditor::currentLine()
{
    int line, index;
    getCursorPosition(&line, &index);
    return line;
}

bool FileEditor::findFirstOccurence(const QString &word, bool caseSensitive, bool wholeWord, bool forward, int line)
{
    // Forward: Begin search from current position
    // Backward: Start search in front of current selection
    int startCursorPosition = -1;
    if(!forward){
        int lineFrom, lineTo, indexTo;
        getSelection(&lineFrom, &startCursorPosition, &lineTo, &indexTo);
    }

    if(text().contains(word))
        return findFirst(word, false, caseSensitive, wholeWord, true, forward, line, startCursorPosition, true, false);

    return false;
}

bool FileEditor::findNextOccurence()
{
    return findNext();
}

void FileEditor::closeEvent(QCloseEvent *event)
{
    if (mChanged){
        QString msg = tr("There are unsaved changes in the file %1.\nDo you want to save it?").arg(filename());
        int button = QMessageBox::question(this, tr("Unsaved File"), msg, QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(button == QMessageBox::Cancel){
            event->ignore();
            return;
        } else if (button == QMessageBox::Yes)
            saveContent();
    }
    event->accept();
}

void FileEditor::focusInEvent(QFocusEvent *event)
{
    // Prevent focus recursion
    if(!mFocusRecursion && !mOmitReloadChecks){
        mFocusRecursion = true;
        emit checkReloadContent();
        mFocusRecursion = false;
    }
    // Propagate event
    QsciScintilla::focusInEvent(event);
}

void FileEditor::keyPressEvent(QKeyEvent *event)
{
    if(!isReadOnly()){
        if (event->key() == Qt::Key_Plus && (event->modifiers() & Qt::ControlModifier)){
            emit increaseFontSizeRequested();
        } else if (event->key() == Qt::Key_Minus && (event->modifiers() & Qt::ControlModifier)){
            emit decreaseFontSizeRequested();
        } else if (event->key() == Qt::Key_0 && (event->modifiers() & Qt::ControlModifier)){
            emit normalizeFontSizeRequested();
        } else if (event->key() == Qt::Key_7 && (event->modifiers() & Qt::ControlModifier)){
            commentSelection();
        } else if (event->key() == Qt::Key_8 && (event->modifiers() & Qt::ControlModifier)){
            uncommentSelection();
        } else if (event->key() == Qt::Key_Space && (event->modifiers() & Qt::ControlModifier)){
            autoCompleteFromAll();
        } else {
            QsciScintilla::keyPressEvent(event);
        }
    } else {
        // Propagate event
        QsciScintilla::keyPressEvent(event);
    }
}

void FileEditor::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier){
        int numDegrees = event->delta() / 8;
        if (numDegrees > 0)
            emit increaseFontSizeRequested();
        else
            emit decreaseFontSizeRequested();
        event->accept();
    } else {
        // Propagate event
        QsciScintilla::wheelEvent(event);
    }
}

void FileEditor::changeEvent(QEvent *event)
{
    QsciScintilla::changeEvent(event);
}

void FileEditor::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = QsciScintilla::createStandardContextMenu();

    if (menu){
        // Add format menu
        menu->addSeparator();
        QMenu *formatMenu = menu->addMenu(tr("Format"));
        formatMenu->addAction(QIcon(":ProjectEditor/table_row_delete.png"), "Comment selection", this, SLOT(commentSelection()), QKeySequence(Qt::CTRL | Qt::Key_7));
        formatMenu->addAction(QIcon(":ProjectEditor/table_row_insert.png"), "Uncomment selection", this, SLOT(uncommentSelection()), QKeySequence(Qt::CTRL | Qt::Key_8));

        // Display
        menu->setAttribute(Qt::WA_DeleteOnClose);
        menu->popup(event->globalPos());
    }
}

void FileEditor::prependStringToSelection(const QString &t)
{
    int lineFrom, indexFrom, lineTo, indexTo;
    getSelection(&lineFrom, &indexFrom, &lineTo, &indexTo);
    for(int line = lineFrom; line <= lineTo; line++)
        insertAt(t, line, 0);
}

void FileEditor::deleteStringInSelection(const QString &t)
{
    int lineFrom, indexFrom, lineTo, indexTo;
    getSelection(&lineFrom, &indexFrom, &lineTo, &indexTo);
    for(int line = lineFrom; line <= lineTo; line++){
        if(text(line).length() >= t.length()){
            // Select a text with lengt of t from line beginning
            setSelection(line, 0, line, t.length());
            // Compare selection and remove if necessary
            if(selectedText().compare(t) == 0)
                replaceSelectedText("");
        }
    }
}

void FileEditor::updateWindowTitle()
{
    QString title = QString(filename()).remove(QString("%1/").arg(mBaseDir));
    if(!QFileInfo(filename()).isWritable())
        title.append(" (Read-Only)");
    else if(mChanged)
        title.append("*");
    mdiWindow->setWindowTitle(title);

}

void FileEditor::updateMargins()
{
    setMarginWidth(LINENUMBER_MARGIN, QString(" %1").arg(lines()));
}

void FileEditor::updateLexer()
{
    // Choose lexer depending on content type
    QString suffix = QFileInfo(filename()).completeSuffix().toLower();
    QsciLexer *lexer = 0;
    if(suffix.compare("cpp") == 0 || suffix.compare("hpp") == 0 || suffix.compare("h") == 0){
        lexer = new QsciLexerCPP();
    }
    if(suffix.compare("py") == 0){
        lexer = new QsciLexerPython();

        // For python we have an predefined API
        QsciAPIs *api = new QsciAPIs(lexer);
        QString filename = QCoreApplication::applicationDirPath() + "/plugins/hilec/python/api/Python-3.4.api";
        api->load(filename);
        api->prepare();
    }
    if(suffix.compare("xml") == 0 || suffix.compare("ogr") == 0){
        lexer = new QsciLexerXML();
    }
    if(lexer){
        lexer->setFont(QFont("Courier"));
        lexer->setDefaultFont(QFont("Courier"));
        setLexer(lexer);
    }
}

void FileEditor::setupEditor()
{
    // Line numbers
    QFont font("Courier");
    setMarginsFont(font);
    setMarginLineNumbers(LINENUMBER_MARGIN, true);
    setMarginWidth(LINENUMBER_MARGIN, QString("%1").arg(lines()) + 6);
    setMarginMarkerMask(1, 0);

    // Font
    font.setStyleHint(QFont::TypeWriter);
    setFont(font);

    // Line selection
    setCaretLineVisible(true);
    setCaretLineBackgroundColor(QColor("#dddddd"));

    // Completions
    setAutoCompletionSource(AcsAll);
    setAutoCompletionUseSingle(AcusExplicit);
    setAutoCompletionCaseSensitivity(false);
    setAutoCompletionThreshold(0);

    // Misc
    setBraceMatching(SloppyBraceMatch);
    setAutoIndent(true);
    setUtf8(true);
    setWrapMode(WrapNone);
    setTabWidth(2);

    // Folding
    setFolding(BoxedTreeFoldStyle, 3);
    setMarginWidth(3, 12);

    // Clickable margins for breakpoints
    setMarginMarkerMask(0, 2);
    setMarginWidth(0, 20);
    setMarginLineNumbers(0, false);
    setMarginSensitivity(BREAKPOINT_MARGIN, true);

    markerDefine(Circle, BREAKPOINTMARKER_NUMBER);
    setMarkerBackgroundColor(QColor(Qt::blue).lighter(), BREAKPOINTMARKER_NUMBER);
    markerDefine(RightArrow, CURRENT_BREAKPOINTMARKER_NUMBER);
    setMarkerBackgroundColor(QColor(Qt::red), CURRENT_BREAKPOINTMARKER_NUMBER);

    int marginMaskBreakpoint = (1 << BREAKPOINTMARKER_NUMBER) | (1 << CURRENT_BREAKPOINTMARKER_NUMBER);
    setMarginMarkerMask(0, marginMaskBreakpoint);

    // Error, warning, ... margins and marker
    setMarginWidth(INFO_MARGIN, 20);
    setMarginSensitivity(INFO_MARGIN, true);
    markerDefine(QImage(":/ProjectEditor/exclamation.png"), ERRORMARKER_NUMBER);
    markerDefine(QImage(":/ProjectEditor/error.png"), EXCLAMATIONMARKER_NUMBER);
    markerDefine(QImage(":/ProjectEditor/information.png"), INFORMATIONMARKER_NUMBER);

    int marginMaskParse = (1 << ERRORMARKER_NUMBER) | (1 << EXCLAMATIONMARKER_NUMBER) | (1 << INFORMATIONMARKER_NUMBER);
    setMarginMarkerMask(INFO_MARGIN, marginMaskParse);


}

void FileEditor::toggleBreakpoint()
{
    int line, index;
    getCursorPosition(&line, &index);
    toggleBreakpoint(line);
}

void FileEditor::toggleBreakpoint(int line)
{
    bool breakpointFound = false;
    foreach(int markerHandle, mBreakpointMarkers.keys())
    {
        if(markerLine(markerHandle) == line)
        {
            // there is a breakpoint here
            mBreakpointMarkers.remove(markerHandle);
            markerDeleteHandle(markerHandle);
            emit removeBreakpoint(mFilename, line+1); // python line starts at 1
            breakpointFound = true;
        }
    }
    if(!breakpointFound)
    {
        int markerId = markerAdd(line, BREAKPOINTMARKER_NUMBER);
        mBreakpointMarkers.insert(markerId, line);
        emit addBreakpoint(mFilename, line+1);  // python line starts at 1
    }
}

void FileEditor::checkBreakpoints()
{
    foreach(int markerId, mBreakpointMarkers.keys())
    {
        int lastLine = mBreakpointMarkers.value(markerId);
        int currentLine = markerLine(markerId);
        if(lastLine != currentLine){
            removeBreakpoint(mFilename, lastLine + 1);
            if(currentLine < 0){ // the marker is not valid anymore
                mBreakpointMarkers.remove(markerId);
            } else{
                mBreakpointMarkers[markerId] = currentLine;
                emit addBreakpoint(mFilename, currentLine + 1);
            }
        }
    }
}

void FileEditor::on_margin_clicked(int margin, int line, Qt::KeyboardModifiers)
{
    // editor count starts at 0, python lien cout starts at 1
    if(margin == BREAKPOINT_MARGIN || margin == LINENUMBER_MARGIN){
        toggleBreakpoint(line);
    } else if(margin == INFO_MARGIN){
         if(mErrors.contains(line+1))  // python line starts at 1
            emit clickedProblem(mFilename,line+1);
    }
}

void FileEditor::on_check_reload_content()
{
    if(mOmitReloadChecks)
        return;

    QFileInfo info(filename());
    if(!info.exists()) // this file was deleted or moved
    {
        mOmitReloadChecks = true;
        QString msg = tr("The file %1 was does not exist anymore.\nDo you want to keep this file in the editor?").arg(filename());
        int button = QMessageBox::question(this, tr("Keep non existing file"), msg, QMessageBox::Yes | QMessageBox::No);
        if(button == QMessageBox::Yes)
           on_text_changed();
        else
           emit close(filename());
        return;
    }
    QDateTime lastModified = info.lastModified();
    if(mLastModified != lastModified){
        QString msg = tr("The file %1 was changed outside the editor.\nDo you want to reload it?").arg(filename());
        int button = QMessageBox::question(this, tr("File was changed outside Editor"), msg, QMessageBox::Yes | QMessageBox::No);
        if(button == QMessageBox::Yes)
            reloadContent();
    }
    mLastModified = lastModified;
}

void FileEditor::on_text_changed()
{
    if (!mdiWindow)
        return;

    // Set flag and window title
    if (!mChanged){
        mChanged = true;
        updateWindowTitle();
    }
}

void FileEditor::on_check_compile_error(const ScriptCompileInfo &info)
{
    if(info.file != mFilename)
        return; // We are not interested in the received data

    // Remove all markers
    markerDeleteAll(EXCLAMATIONMARKER_NUMBER);
    markerDeleteAll(ERRORMARKER_NUMBER);
    markerDeleteAll(INFORMATIONMARKER_NUMBER);

    // Clear "error memory"
    mErrors.clear();

    // Create "error memory"
    foreach(ScriptCompileProblem problem, info.problems){
        bool insert = true;
        if(mErrors.contains(problem.line)){
            if(mErrors[problem.line].mode <= problem.mode)
                insert = false;
            else
                // Problem has higher priority, remove current marker
                markerDelete(problem.line-1, ERRORMARKER_NUMBER + mErrors[problem.line].mode);
        }

        if(insert){
            mErrors.insert(problem.line, problem);
            // Remove eventually current marker
            markerAdd(problem.line-1, ERRORMARKER_NUMBER + problem.mode);
        }
    }
}


