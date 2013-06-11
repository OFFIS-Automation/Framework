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

#ifndef FILEEDITOR_H
#define FILEEDITOR_H

#include <QMdiSubWindow>
#include <QDateTime>
#include <QMap>

#define QSCINTILLA_DLL
#include <Qsci/qsciscintilla.h>

// Forward declaration of compile info, may contain warning, errors, ...
struct ScriptCompileInfo;

class FileEditor : public QsciScintilla
{
    Q_OBJECT

public:
    FileEditor(const QString& filename, QMdiSubWindow *parent = 0);
    QString filename() { return mFilename; }


signals:
    void createBreakpoint(QString file, int line);
    void removeBreakpoint(QString file, int line);
    void checkReloadContent();
    void saveFileRequested();
    void increaseFontSizeRequested();
    void decreaseFontSizeRequested();
    void asyncRemoveChangeFlag();

public slots:
    bool saveContent();
    void reloadContent();
    void focusLine(int line);
    void setFontSize(int fontSize);
    void removeChangedFlag();
    int currentLine();

    bool findFirstOccurence(const QString &expr, bool caseSensitive, bool wholeWord, bool forward, int line);
    bool findNextOccurence();

protected:
    void closeEvent(QCloseEvent *event);
    void focusInEvent(QFocusEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

    void prependStringToSelection(const QString& t);
    void deleteStringInSelection(const QString& t);

private slots:
    void updateLexer();
    void setupEditor();
    void toggleBreakpoint();

    void on_margin_clicked(int margin, int line, Qt::KeyboardModifiers modifiers);
    void on_check_reload_content();
    void on_text_changed();
    void on_check_compile_error(const ScriptCompileInfo& info);

private:
    QMdiSubWindow* mdiWindow;
    QString mFilename;
    QDateTime mLastModified;
    QList<int> mBreakpoints;

    struct CompileInfo{
        QString msg;
        int priority;
    };
    QMap<int, CompileInfo> mErrors;

    int mCurrentBreakpoint;
    bool mChanged;
    bool mFocusRecursion;
};

#endif // FILEEDITOR_H
