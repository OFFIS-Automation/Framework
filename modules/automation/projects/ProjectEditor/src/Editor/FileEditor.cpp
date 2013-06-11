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

#include "FileEditor.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QFont>
#include <QFontMetrics>
#include <QShortcut>
#include <QMessageBox>
#include <QCoreApplication>
#include <QToolTip>
#include "qevent.h"

#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qscilexerxml.h>
#include <Qsci/qsciapis.h>

#include "../HilecSingleton.h"
#include "core/ScriptException.h"

#define BREAKPOINT_MARGIN 0
#define LINENUMBER_MARGIN 1
#define INFO_MARGIN 2
#define BREAKPOINTMARKER_NUMBER 1
#define CURRENT_BREAKPOINTMARKER_NUMBER 2
#define ERRORMARKER_NUMBER 3
#define EXCLAMATIONMARKER_NUMBER 4
#define INFORMATIONMARKER_NUMBER 5


FileEditor::FileEditor(const QString &filename, QMdiSubWindow *parent) :
    mFilename(filename), mdiWindow(parent)
{
    mFocusRecursion = mChanged = false;
    mCurrentBreakpoint = -1;

    // Update GUI and content
    reloadContent();
    setupEditor();
    updateLexer();

    // Connections
    connect(this, SIGNAL(checkReloadContent()), SLOT(on_check_reload_content()), Qt::QueuedConnection);
    connect(this, SIGNAL(textChanged()), SLOT(on_text_changed()));
    connect(this, SIGNAL(asyncRemoveChangeFlag()), SLOT(removeChangedFlag()), Qt::QueuedConnection);

    // Breakpoint
    connect(this, SIGNAL(marginClicked(int,int,Qt::KeyboardModifiers)), SLOT(on_margin_clicked(int,int,Qt::KeyboardModifiers)));
    connect(this, SIGNAL(createBreakpoint(QString,int)), HilecSingleton::hilec(), SLOT(addBreakpoint(QString,int)));
    connect(this, SIGNAL(removeBreakpoint(QString,int)), HilecSingleton::hilec(), SLOT(removeBreakpoint(QString,int)));

    // Compile error
    connect(HilecSingleton::hilec(), SIGNAL(compileError(ScriptCompileInfo)), SLOT(on_check_compile_error(ScriptCompileInfo)));

    // Keyboard Shortcurts
    new QShortcut(Qt::Key_F9, this, SLOT(toggleBreakpoint()));
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
    emit asyncRemoveChangeFlag();
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
    if(line < 0){
        // Line is invalid, remove focus
        mCurrentBreakpoint = -1;
        markerDeleteAll(CURRENT_BREAKPOINTMARKER_NUMBER);
    } else {
        // Add marker
        mCurrentBreakpoint = line;
        markerAdd(line-1, CURRENT_BREAKPOINTMARKER_NUMBER);
        // Set focus of cursor
        setCursorPosition(line, 0);
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
        QString title = mdiWindow->windowTitle();
        if(title.endsWith("*"))
            title.chop(1);
        mdiWindow->setWindowTitle(title);
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
    if(!mFocusRecursion){
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
        // Check for new line
        setMarginWidth(LINENUMBER_MARGIN, QString("%1").arg(lines()) + 6);

        // Modifier stuff
        if (event->key() == Qt::Key_Plus && (event->modifiers() & Qt::ControlModifier))
            emit increaseFontSizeRequested();
        else if (event->key() == Qt::Key_Minus && (event->modifiers() & Qt::ControlModifier))
            emit decreaseFontSizeRequested();
        else if (event->key() == Qt::Key_7 && (event->modifiers() & Qt::ControlModifier))
            prependStringToSelection("#");
        else if (event->key() == Qt::Key_8 && (event->modifiers() & Qt::ControlModifier))
            deleteStringInSelection("#");
        else if (event->key() == Qt::Key_Space && (event->modifiers() & Qt::ControlModifier))
            autoCompleteFromAll();
        else
            QsciScintilla::keyPressEvent(event);
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

void FileEditor::updateLexer()
{
    // Choose lexer depending on content type
    QString suffix = QFileInfo(filename()).completeSuffix();
    QsciLexer *lexer = 0;
    if(suffix.compare("cpp") == 0 || suffix.compare("h") == 0)
        lexer = new QsciLexerCPP();
    if(suffix.compare("py") == 0){
        lexer = new QsciLexerPython();

        // For python we have an predefined API
        QsciAPIs *api = new QsciAPIs(lexer);
        QString filename = QCoreApplication::applicationDirPath() + "/plugins/hilec/python/api/Python-3.1.api";
        api->load(filename);
        api->prepare();
    }
    if(suffix.compare("xml") == 0 || suffix.compare("ogr") == 0)
        lexer = new QsciLexerXML();

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
    setAutoCompletionShowSingle(true);
    setAutoCompletionCaseSensitivity(false);
    setAutoCompletionThreshold(2);

    // Misc
    setBraceMatching(SloppyBraceMatch);
    setAutoIndent(true);
    setUtf8(true);
    setFolding(CircledTreeFoldStyle, 2);
    setWrapMode(WrapWord);

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
    setMarginType(INFO_MARGIN, SymbolMargin);
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
    on_margin_clicked(BREAKPOINT_MARGIN, line, 0);
}

void FileEditor::on_margin_clicked(int margin, int line, Qt::KeyboardModifiers modifiers)
{
    if(margin == BREAKPOINT_MARGIN || margin == LINENUMBER_MARGIN){
        if(!mBreakpoints.contains(line)){
            // Add line to breakpoints and emit signal
            mBreakpoints << line;
            emit createBreakpoint(mFilename, line);
            // Add marker to margin
            markerAdd(line, BREAKPOINTMARKER_NUMBER);
        } else {
            // Remove line from breakpoints and emit signal
            mBreakpoints.removeAll(line);
            emit removeBreakpoint(filename(), line);
            // Remove marker from margin
            markerDelete(line, BREAKPOINTMARKER_NUMBER);
        }
    }
}

void FileEditor::on_check_reload_content()
{
    QDateTime lastModified = QFileInfo(filename()).lastModified();
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
        mdiWindow->setWindowTitle(mdiWindow->windowTitle().append("*"));
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
        // Generate info
        CompileInfo compileInfo;
        compileInfo.msg = problem.msg;
        compileInfo.priority = problem.mode;
        bool insert = true;
        if(mErrors.contains(problem.line)){
            if(mErrors[problem.line].priority <= problem.mode)
                insert = false;
            else
                // Problem has higher priority, remove current marker
                markerDelete(problem.line-1, ERRORMARKER_NUMBER + mErrors[problem.line].priority);
        }

        if(insert)
            mErrors.insert(problem.line, compileInfo);

        // Remove eventually current marker
        markerAdd(problem.line-1, ERRORMARKER_NUMBER + compileInfo.priority);
    }
}


