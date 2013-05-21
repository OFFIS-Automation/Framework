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

#include "CodeEditor.h"
#include "LineNumberArea.h"
#include "../HilecSingleton.h"

#include "SyntaxHighlighterFactory.h"

#include <QMessageBox>
#include <QDebug>
#include <QFileInfo>
#include <QSettings>
#include <QToolTip>
#include <QShortcut>

#include <core/ScriptException.h>

CodeEditor::CodeEditor(const QString &filename, QMdiSubWindow *parent) :
    QPlainTextEdit(parent), mFilename(filename)
{
    mErrorImages[ScriptCompileProblem::Error] = QImage(":/ProjectEditor/exclamation.png");
    mErrorImages[ScriptCompileProblem::Warning] = QImage(":/ProjectEditor/error.png");
    mErrorImages[ScriptCompileProblem::Info] = QImage(":/ProjectEditor/information.png");
    mFocusRecursion = false;
    mChanged = false;
    mCurrentBreakpoint = -1;
    mdiWindow = parent;
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    connect(this, SIGNAL(textChanged()), SLOT(setTextChanged()));
    connect(this, SIGNAL(checkReload()), SLOT(onCheckReload()), Qt::QueuedConnection);

    connect(this, SIGNAL(createBreakpoint(QString,int)), HilecSingleton::hilec(), SLOT(addBreakpoint(QString,int)));
    connect(this, SIGNAL(removeBreakpoint(QString,int)), HilecSingleton::hilec(), SLOT(removeBreakpoint(QString,int)));

    connect(HilecSingleton::hilec(), SIGNAL(compileError(ScriptCompileInfo)), SLOT(checkCompileError(ScriptCompileInfo)));

    connect(this, SIGNAL(asyncRemoveChangeFlag()), SLOT(removeChangedFlag()), Qt::QueuedConnection);
    updateLineNumberAreaWidth(0);
    //setCenterOnScroll(true);
    highlightCurrentLine();

    setLineWrapMode(NoWrap);
    QFont font("Courier");
    font.setStyleHint(QFont::TypeWriter);
    setFont(font);
    QPalette p = palette();
    p.setColor(QPalette::Highlight, QColor(Qt::blue).lighter(80));
    p.setColor(QPalette::HighlightedText, Qt::white);
    setPalette(p);
    QSettings settings;
    setFontSize(settings.value("ProjectEditor/fontsize", 12).toInt());
    SyntaxHighlighterFactory::create(filename, document());

    reload();
    new QShortcut(Qt::Key_F9, this, SLOT(toggleBreakpoint()));

}


void CodeEditor::focusLine(int line)
{
    if(line < 0)
    {
        mExtraSelection.remove("focus");
        mCurrentBreakpoint = -1;
    }
    else
    {
        QTextCursor c = textCursor();
        int pos = c.blockNumber()+1;
        if(pos > line)
            c.movePosition(QTextCursor::Up,QTextCursor::MoveAnchor,pos-line);
        else
            c.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor,line-pos);
        c.movePosition(QTextCursor::StartOfLine);
        setTextCursor(c);
        QTextEdit::ExtraSelection& selection = mExtraSelection["focus"];
        QColor lineColor = QColor(Qt::blue).lighter(150);
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        mCurrentBreakpoint = line;
    }
    setExtraSelections(mErrorSelections + mExtraSelection.values());
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space + 12;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine()
{
    if (!isReadOnly()) {
        QTextEdit::ExtraSelection& selection = mExtraSelection["currentLine"];

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
    }

    setExtraSelections(mErrorSelections + mExtraSelection.values());
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            int line = blockNumber + 1;

            if(mBreakpoints.contains(line)){
                painter.setPen(Qt::red);
                QRectF rectangle(0, top, lineNumberArea->width(), fontMetrics().height());
                painter.drawRect(rectangle);
            }
            if(mCurrentBreakpoint == line)
                painter.drawImage(0, top, QImage(":/ProjectEditor/arrow_right.png"));
            if(mErrors.contains(line))
            {
                CompileInfo& info = mErrors[line];
                painter.drawImage(0, top, info.image);
                info.point = QPoint(0, top);
            }
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, QString::number(line));
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }

}

void CodeEditor::lineNumberAreaMoveEvent(QMouseEvent *e)
{
    foreach(const CompileInfo& info, mErrors.values())
    {
        QPoint point = this->mapToGlobal(info.point);
        QPoint mousePoint = this->mapToGlobal(e->pos());

        if (point.x() < mousePoint.x() && point.x() + 16 > mousePoint.x() &&
                point.y() < mousePoint.y() && point.y() + 16 > mousePoint.y())
        {
            QToolTip::showText(mousePoint, info.msg, this);
            break;
        }
    }
}

void CodeEditor::lineNumberAreaPressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        e->accept();
        int y = e->y();
        //int contentsY = codeEditor->verticalScrollBar()->value();
        QTextBlock block = firstVisibleBlock();
        int lineNumber = block.blockNumber()+1;
        int linePosMin = 0;
        int linePosMax;
        for ( ; block.isValid(); block = block.next())
        {
            if(!block.isVisible())
                continue;
            QTextLayout* layout = block.layout();
            const QRectF boundingRect = layout->boundingRect();
            linePosMax = boundingRect.height() + linePosMin;
            if ((linePosMax> y) && (linePosMin < y)) {
                if (mBreakpoints.contains(lineNumber))
                {
                    mBreakpoints.removeOne(lineNumber);
                    emit removeBreakpoint(filename(), lineNumber);
                }
                else
                {
                    mBreakpoints.append(lineNumber);
                    emit createBreakpoint(filename(), lineNumber);
                }
                break;
            }
            linePosMin = linePosMax;
            lineNumber++;
        }
    }

    repaint();
}
void CodeEditor::keyPressEvent(QKeyEvent *e)
{

    if(isReadOnly())
    {
        QPlainTextEdit::keyPressEvent(e);
        return;
    }

    if(e->key() == Qt::Key_Tab && !(e->modifiers() & Qt::ControlModifier) && textCursor().hasSelection())
        prependStringToSelection("\t");
    else if(e->key() == Qt::Key_Backtab && !(e->modifiers() & Qt::ControlModifier))
        deleteStringInSelection("\t");
    else if ((e->key() == Qt::Key_Plus || e->key() == Qt::Key_BracketRight) &&
             (e->modifiers() == Qt::ControlModifier || e->modifiers() == (Qt::KeypadModifier | Qt::ControlModifier)))
        emit increaseFontSizeRequested();
    else if (e->key() == Qt::Key_Minus && (e->modifiers() & Qt::ControlModifier))
        emit decreaseFontSizeRequested();
    else if (e->key() == Qt::Key_7 && (e->modifiers() & Qt::ControlModifier))
        prependStringToSelection("#");
    else if (e->key() == Qt::Key_8 && (e->modifiers() & Qt::ControlModifier))
        deleteStringInSelection("#");

    else
    {
        QPlainTextEdit::keyPressEvent(e);
        if (e->key()==Qt::Key_Return)
            formatNewLine();
    }
}

void CodeEditor::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() & Qt::ControlModifier)
    {
        int numDegrees = e->delta() / 8;
        if (numDegrees > 0)
            emit increaseFontSizeRequested();
        else
            emit decreaseFontSizeRequested();
        e->accept();
    }
    else
        QPlainTextEdit::wheelEvent(e);
}

void CodeEditor::formatNewLine()
{
    QTextCursor currentCursor = textCursor();

    currentCursor.movePosition(QTextCursor::PreviousBlock);
    currentCursor.movePosition(QTextCursor::StartOfLine);
    currentCursor.select(QTextCursor::LineUnderCursor);
    QString str = currentCursor.selectedText();
    QString add = "";
    if(str.trimmed().right(1) == ":"){
        add+="\t";
    }
    int pos = 0;
    for(int i=0;i<str.length();++i){
        if(str.at(i) != '\t' && str.at(i) != ' '){
            break;
        }
        pos = i+1;
    }
    insertPlainText(str.mid(0,pos)+add);
}

void CodeEditor::prependStringToSelection(const QString& text)
{
    QTextCursor c = textCursor();
    if(!c.hasSelection())
        c.select(QTextCursor::LineUnderCursor);

    int start = c.selectionStart();
    int end = c.selectionEnd();

    c.beginEditBlock();
    c.setPosition(start);
    c.movePosition(QTextCursor::StartOfBlock);
    c.movePosition(QTextCursor::StartOfLine);
    int last = -1;
    while(c.position() <= end && c.position() != last)
    {
        c.insertText(text);
        last = c.position();
        end+=text.length();
        if(!c.movePosition(QTextCursor::NextBlock))
            break;
        c.movePosition(QTextCursor::StartOfLine);
    }
    c.endEditBlock();
}

void CodeEditor::deleteStringInSelection(const QString &text)
{
    QTextCursor c = textCursor();
    if(!c.hasSelection())
        c.select(QTextCursor::LineUnderCursor);
    int start = c.selectionStart();
    int end = c.selectionEnd();
    c.beginEditBlock();
    c.setPosition(start);
    c.movePosition(QTextCursor::StartOfBlock);
    c.movePosition(QTextCursor::StartOfLine);
    int last = -1;
    int current;
    while(c.position() <= end && c.position() != last)
    {
        last = c.position();
        c.movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,text.length());
        if(c.selectedText() == text) {
                c.removeSelectedText();
                end-= text.length();
        }
        if(!c.movePosition(QTextCursor::NextBlock))
            break;
        c.movePosition(QTextCursor::StartOfLine);
        current = c.position();
    }
    c.endEditBlock();
}

void CodeEditor::setTextChanged()
{
    if (!mdiWindow)
        return;
    if (!mChanged)
    {
        mChanged = true;
        mdiWindow->setWindowTitle(mdiWindow->windowTitle().append("*"));
    }
}

void CodeEditor::removeChangedFlag()
{
    if (!mdiWindow)
        return;
    if (mChanged)
    {
        mChanged = false;
        QString title = mdiWindow->windowTitle();
        if(title.endsWith("*"))
            title.chop(1);
        mdiWindow->setWindowTitle(title);
    }
}

void CodeEditor::closeEvent(QCloseEvent *event)
{
    if (mChanged)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Unsaved file!"),
            tr("This file wasn't saved! Do you want to save it?"),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(reply == QMessageBox::Cancel)
        {
            event->ignore();
            return;
        }
        if (reply == QMessageBox::Yes)
            save();
    }
    event->accept();
}

void CodeEditor::checkCompileError(const ScriptCompileInfo &info)
{
    if(info.file != mFilename)
        return;
    mErrorSelections.clear();
    mErrors.clear();
    foreach(ScriptCompileProblem err, info.problems)
    {
        CompileInfo info;
        info.msg = err.msg;
        info.image = mErrorImages.value(err.mode);
        info.priority = err.mode;
        bool insert = true;
        if(mErrors.contains(err.line))
            insert = mErrors[err.line].priority >= err.mode;
        if(insert)
            mErrors.insert(err.line, info);

        if(err.mode == ScriptCompileProblem::Error)
        {
            QTextCursor c = textCursor();
            int pos = c.blockNumber()+1;
            if(pos > err.line)
                c.movePosition(QTextCursor::Up,QTextCursor::MoveAnchor,pos-err.line);
            else
                c.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor,err.line-pos);
            c.movePosition(QTextCursor::StartOfLine);
            mErrorSelections.append(QTextEdit::ExtraSelection());
            QTextEdit::ExtraSelection& selection = mErrorSelections.last();
            QColor lineColor = QColor(Qt::red).lighter(150);
            selection.format.setBackground(lineColor);
            selection.format.setProperty(QTextFormat::FullWidthSelection, true);
            selection.cursor = c;
            selection.cursor.clearSelection();
        }
    }
    setExtraSelections(mErrorSelections + mExtraSelection.values());
    update();
}

void CodeEditor::save()
{
    QFile file(filename());
    file.open(QFile::WriteOnly);
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream.setGenerateByteOrderMark(false);
    stream << toPlainText();
    file.close();
    mLastModified = QFileInfo(filename()).lastModified();
    emit asyncRemoveChangeFlag();
}

void CodeEditor::onCheckReload()
{
    QDateTime lastModified = QFileInfo(filename()).lastModified();

    if(mLastModified != lastModified)
    {
        QString msg = tr("The file %1 was changed outside the editor.\nDo you wish to reload the file?").arg(filename());
        int button = QMessageBox::question(this, tr("File was changed outside Editor"), msg, QMessageBox::Yes | QMessageBox::No);
        if(button == QMessageBox::Yes)
            reload();
    }
    mLastModified = lastModified;
}

void CodeEditor::reload()
{
    mLastModified = QFileInfo(filename()).lastModified();
    QFile file(filename());
    file.open(QFile::ReadOnly);
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    setPlainText(stream.readAll());
    file.close();
    emit asyncRemoveChangeFlag();
}

void CodeEditor::focusInEvent(QFocusEvent* e)
{
    if(!mFocusRecursion)
    {
        mFocusRecursion = true;
        emit checkReload();
        mFocusRecursion = false;
    }
    QPlainTextEdit::focusInEvent(e);
}

void CodeEditor::search(QString word, int number)
{
    if (toPlainText().contains(word))
    {
        int pos = 0;
        for (int i = 0; i < number; i++)
        {
            pos = toPlainText().indexOf(word, pos + word.size());
            if (pos == -1)
                pos = 0;
        }
        if (pos == 0)
            pos = toPlainText().indexOf(word);
        QTextCursor c = textCursor();
        c.setPosition(pos);
        c.movePosition(QTextCursor::Right ,QTextCursor::KeepAnchor, word.size());
        QTextEdit::ExtraSelection& selection = mExtraSelection["error"];
        QColor lineColor = QColor(Qt::green).lighter(150);
        selection.format.setBackground(lineColor);
        selection.cursor = c;
        setTextCursor(c);
        setExtraSelections(mErrorSelections + mExtraSelection.values());
    }
}

void CodeEditor::replace(QString word)
{
    QTextCursor c = textCursor();
    if (c.hasSelection())
    {
        c.removeSelectedText();
        c.clearSelection();
        c.insertText(word);
        setTextCursor(c);
    }
}

void CodeEditor::toggleBreakpoint()
{
    int line = textCursor().blockNumber()+1;
    if(!mBreakpoints.contains(line))
    {
        mBreakpoints << line;
        emit createBreakpoint(filename(), line);
    }
    else
    {
        mBreakpoints.removeAll(line);
        emit removeBreakpoint(filename(), line);
    }
    repaint();
}

void CodeEditor::setFontSize(int fontSize)
{
    QFont f = font();
    f.setPointSize(fontSize);
    setFont(f);
    setTabStopWidth(fontMetrics().width(QChar('0'))*3);
}
