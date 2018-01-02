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

#include "LogWindow.h"
#include "ui_LogWindow.h"

#include <QMainWindow>
#include <QLabel>


LogWindow* LogWindow::sInstance = 0;
QMutex LogWindow::sMutex(QMutex::Recursive);
LogWindow::LogWindow(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::LogWindow)
{
    QMutexLocker lock(&sMutex);
    if(sInstance != 0)
    {
        qFatal("Log Window cannot be instantiated more than once");
    }
    ui->setupUi(this);

    mLogFile.setFileName(QCoreApplication::applicationDirPath() + "/logfile.log");
    mLogFile.open(QFile::Append | QFile::Text | QIODevice::Unbuffered);
    mLogStream.setDevice(&mLogFile);

    ui->plainTextEdit->setMaximumBlockCount(200);
    mHighlighter = new LogSyntaxHighlighter(ui->plainTextEdit->document());

    addLogType(QtDebugMsg, tr("Debug"), "DBG", Qt::blue);
    addLogType(QtWarningMsg, tr("Warning"), "WRN", Qt::darkYellow);
    addLogType(QtCriticalMsg, tr("Error"), "ERR", Qt::red);
    addLogType(QtInfoMsg, tr("Info"), "INF", QColor(64, 64, 64));

    sInstance = this;
    mPreviousHandler = qInstallMessageHandler(&LogWindow::logMessage);
    mTimer.setSingleShot(true);
    mTimer.start(300);
    connect(&mTimer,SIGNAL(timeout()), SLOT(onTimeout()));
}

LogWindow::~LogWindow()
{
    {
        QMutexLocker lock(&sMutex);

        qInstallMessageHandler(mPreviousHandler);
        sInstance = 0;
        mTimer.stop();
    }
    onTimeout();
    delete ui;
}

void LogWindow::logMessage(QtMsgType type, const QMessageLogContext &context, const QString& msg)
{
    QMutexLocker lock(&sMutex);
    if(!sInstance)
        return;
    sInstance->addMessage(type, context, msg);
}

void LogWindow::addLogType(int id, const QString &name, const QString &prefix, const QColor &color)
{
    mHighlighter->addType(prefix, color);

    LogTypeStruct& type = mLogTypes[id];
    type.prefix = "[" + prefix + "] ";
    type.edit = new ExtendedTextEdit();
    type.edit->setReadOnly(true);
    type.edit->setMaximumBlockCount(200);

    LogSyntaxHighlighter* highlighter = new LogSyntaxHighlighter(type.edit->document());
    highlighter->addType(prefix, color);
    ui->tabWidget->addTab(type.edit, name);
}

void LogWindow::addMessage(QtMsgType type, const QMessageLogContext & /*context */, const QString &msg)
{
    QString prefix;
    if(mLogTypes.contains(type))
    {
        LogTypeStruct& logType = mLogTypes[type];
        prefix = logType.prefix;
        logType.buffer << prefix + msg;
    }
    else
    {
        prefix = "[USR] ";
    }
    mLogStream << prefix << msg << endl;
    mBuffer << prefix + msg;
}

void LogWindow::onTimeout()
{
    QMap<QPlainTextEdit*, QStringList> updates;
    QStringList buffer;
    {
        // lock scope
        QMutexLocker lock(&sMutex);
        buffer = mBuffer;
        mBuffer = QStringList();
        mAllBuffer = QString();

        QMutableMapIterator<int, LogTypeStruct> messages(mLogTypes);
        while(messages.hasNext())
        {
            LogTypeStruct& logType = messages.next().value();
            if(logType.buffer.isEmpty())
                continue;
            updates.insert(logType.edit, logType.buffer);
            logType.buffer = QStringList();
        }
    }
    int maxBlocks = ui->plainTextEdit->maximumBlockCount();
    int startElement = qMax(0, buffer.size()-maxBlocks);
    for(int i=startElement; i<buffer.size(); i++)
    {
        ui->plainTextEdit->appendPlainText(buffer[i]);
    }
    QApplication::processEvents();
    QMapIterator<QPlainTextEdit*, QStringList> iter(updates);
    while(iter.hasNext())
    {
        iter.next();
        QPlainTextEdit* edit = iter.key();
        const QStringList& dataBuffer = iter.value();
        maxBlocks = edit->maximumBlockCount();
        startElement = qMax(0, dataBuffer.size()-maxBlocks);
        for(int i=startElement; i<dataBuffer.size(); i++)
        {
            edit->appendPlainText(dataBuffer[i]);
        }
        QApplication::processEvents();
    }
    mTimer.start(300);
}
