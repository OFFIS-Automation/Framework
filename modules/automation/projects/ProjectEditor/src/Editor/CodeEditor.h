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

#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QPainter>
#include <QTextBlock>
#include <QScrollBar>
#include <QMdiSubWindow>
#include <QDateTime>
#include <QSyntaxHighlighter>

struct ScriptCompileInfo;

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit CodeEditor(const QString& filename, QMdiSubWindow *parent = 0);
    QString filename() { return mFilename; }
    void setFilename(QString name) { mFilename = name; }
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    void lineNumberAreaMoveEvent(QMouseEvent *e);
    void lineNumberAreaPressEvent(QMouseEvent *e);
    int lineNumberAreaWidth();
    void setHighlighter(QSyntaxHighlighter *highlighter);
    void setWindow(QMdiSubWindow *window);

signals:
    void createBreakpoint(QString file, int line);
    void removeBreakpoint(QString file, int line);
    void saveFileRequested();
    void increaseFontSizeRequested();
    void decreaseFontSizeRequested();
    void asyncRemoveChangeFlag();
    void checkReload();

public slots:
    void setFontSize(int fontSize);
    void focusLine(int line);
    void removeChangedFlag();
    void checkCompileError(const ScriptCompileInfo& err);
    void save();
    void onCheckReload();
    void reload();
    void search(QString word, int number);
    void replace(QString word);

    void toggleBreakpoint();

protected:
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *e);
    void wheelEvent(QWheelEvent *e);
    void prependStringToSelection(const QString& text);
    void deleteStringInSelection(const QString& text);
    void formatNewLine();
    void closeEvent(QCloseEvent *event);
    void focusInEvent(QFocusEvent *e);
private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
    void setTextChanged();

private:
    QMap<QString, QTextEdit::ExtraSelection> mExtraSelection;
    QList<QTextEdit::ExtraSelection> mErrorSelections;
    QWidget *lineNumberArea;
    QMdiSubWindow* mdiWindow;
    QString mFilename;
    bool mChanged;
    QDateTime mLastModified;
    int mCurrentBreakpoint;
    struct CompileInfo
    {
        QString msg;
        QImage image;
        QPoint point;
        int priority;
    };

    QMap<int, CompileInfo> mErrors;
    QMap<int, QImage> mErrorImages;
    QList<int> mBreakpoints;

    bool mFocusRecursion;
};

#endif // CODEEDITOR_H
