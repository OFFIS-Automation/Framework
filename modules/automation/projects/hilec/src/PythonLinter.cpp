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

#include "PythonLinter.h"
#include <QProcess>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QThreadPool>
#include "HilecCore.h"
#include <core/ScriptException.h>


class PythonLinterRunnable : public QRunnable
{
public:
    PythonLinterRunnable(const QString& filename, const QString& base)
    {
        mFile = filename;
        mBase = base;
    }
    void run()
    {
        QProcess process;
        QString dir = QCoreApplication::applicationDirPath() + "/plugins/hilec";
        process.setWorkingDirectory(dir);
        qDebug() << QFileInfo(mFile).canonicalFilePath();
        qDebug() << mBase;
        process.start(dir + "/compile.bat", QStringList(QFileInfo(mFile).canonicalFilePath()) << mBase);
        if(!process.waitForFinished(5000))
        {
            qDebug() << process.readAllStandardOutput();;
            qWarning() << process.readAllStandardError();
            process.kill();
            return;
        }

        QString str = process.readAllStandardOutput();
        //qDebug() << str;
        QStringList errors = str.split("\n", QString::SkipEmptyParts);
        ScriptCompileInfo info;
        info.file = mFile;
        QDir baseDir(mBase);
        foreach(QString line, errors)
        {
            ScriptCompileProblem problem;
            QStringList parts = line.split(":");
            if(parts.size() < 3)
                continue;
            QString file = parts.takeFirst();
            if(file.size() == 1) // this is a windows disc
                file += ":" + parts.takeFirst();
            if(QFileInfo(baseDir.absoluteFilePath(file)) != QFileInfo(mFile))
                continue;
            if(parts.size() < 2)
                continue;
            problem.line = parts.takeFirst().toInt();
            QString msg = parts.join(":").trimmed().mid(1);
            int pos = msg.indexOf("]");
            if(pos < 1)
                continue;
            QChar mode = msg[0];
            if(mode == 'E' || mode == 'F')
                problem.mode = ScriptCompileProblem::Error;
            else if(mode == 'W')
                problem.mode = ScriptCompileProblem::Warning;
            else if(mode == 'C')
                problem.mode = ScriptCompileProblem::Info;
            else
                continue;
            problem.msg = msg.mid(pos + 1).trimmed();
            info.problems << problem;
            //qDebug() << mode << msg;
        }
        HilecCore::instance().raiseCompileError(info);

    }
protected:
    QString mFile, mBase;
};

PythonLinter::PythonLinter(QObject *parent) :
    QObject(parent)
{
    mThreadPool.setMaxThreadCount(1);
}

void PythonLinter::setBasePath(const QString &base)
{
    mBasePath = base;
}

void PythonLinter::compile(const QString &filename)
{
    mThreadPool.start(new PythonLinterRunnable(filename, mBasePath));
    //PythonLinterRunnable run(filename, mBasePath);
    //run.run();
}
