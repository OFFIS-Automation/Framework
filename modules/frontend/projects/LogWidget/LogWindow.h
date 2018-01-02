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

#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QtCore>
#include <QDockWidget>
#include <QMutex>
#include <QTimer>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include "ExtendedTextEdit.h"
#include "LogSyntaxHighlighter.h"

namespace Ui {
    class LogWindow;
}

class QPlainTextEdit;

class LogWindow : public QDockWidget
{
    Q_OBJECT

public:
    explicit LogWindow(QWidget *parent = 0);
    ~LogWindow();

    static void logMessage(QtMsgType type,const QMessageLogContext& context,const QString& msg);

private slots:
    void onTimeout();

private:
    void addMessage(QtMsgType type, const QMessageLogContext& context, const QString& msg);
    void addLogType(int type, const QString& name, const QString& prefix, const QColor& color);

    static QMutex sMutex;
    static LogWindow* sInstance;
    struct LogTypeStruct
    {
        QString prefix;
        QStringList buffer;
        ExtendedTextEdit* edit;
    };
    QMap<int, LogTypeStruct> mLogTypes;
    QString mAllBuffer;

    QTimer mTimer;
    Ui::LogWindow *ui;
    QtMessageHandler mPreviousHandler;
    QFile mLogFile;
    QTextStream mLogStream;
    QStringList mBuffer;
    QList<QMainWindow*> mParents;
    LogSyntaxHighlighter* mHighlighter;
};

#endif // LOGWINDOW_H
