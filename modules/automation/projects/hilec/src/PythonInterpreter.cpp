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

#include "PythonInterpreter.h"

#include <QDebug>
#include <QDir>
#include <core/ScriptException.h>
#include <lolecs/Pose2d.h>
#include "HilecCore.h"

PythonInterpreter::PythonInterpreter(const QString &configDir)
{
    qRegisterMetaType<Pose2d>("Pose2d");
    qRegisterMetaTypeStreamOperators<Pose2d>("Pose2d");
    mPythonProcess = 0;
    mConfigDir = configDir;
    connect(&mServer, SIGNAL(newConnection()), SLOT(connectScript()));
    mServer.listen("OffisFrameworkPythonSocket");
}

PythonInterpreter::~PythonInterpreter()
{
}

void PythonInterpreter::start(const QString &filename, const QString &baseDir)
{
    qDebug() << "Python: Executing" << filename;
    QString last = QDir::currentPath();
    QDir::setCurrent(mBaseDir);
    //TODO: EXECUTE Script
    QDir::setCurrent(last);
    qDebug() << "Python: Execution finished" << filename;
    HilecCore::instance().scriptPaused(QString(), -1);
}

void PythonInterpreter::addBreakpoint(const QString &file, int line)
{
    //TODO
}

void PythonInterpreter::removeBreakpoint(const QString &file, int line)
{
    //TODO
}

QList<QPair<QString, int> > PythonInterpreter::breakpoints() const
{
    return mBreakpoints;
}

bool PythonInterpreter::wait(int timeout)
{
    //TODO
    return true;
}

bool PythonInterpreter::userInput(int uid, int buttonId, const QList<QVariant> &data)
{
    //TODO
    return true;
}



void PythonInterpreter::resume()
{
    //TODOmDebugger->step(PythonDebugger::Resume);
}

void PythonInterpreter::stepInto()
{
    //TODOmDebugger->step(PythonDebugger::StepInto);
}

void PythonInterpreter::stepOver()
{
    //TODOmDebugger->step(PythonDebugger::StepOver);
}

void PythonInterpreter::stepReturn()
{
    //TODOmDebugger->step(PythonDebugger::StepReturn);
}

void PythonInterpreter::quit()
{
    //TODOUserRequestManager::instance()->abortAll();
    //TODOmDebugger->step(PythonDebugger::Quit);
}

QAbstractItemModel* PythonInterpreter::debugVars(int frameDepth)
{
    //TODOreturn mDebugger->variables(frameDepth);
    return 0;
}

QList<TraceLine> PythonInterpreter::getStackTrace()
{
    //TODOreturn mDebugger->callStack();
    QList<TraceLine> trace;
    return trace;
}

void PythonInterpreter::connectScript()
{
    QMutexLocker lock(&mMutex);
    QLocalSocket* socket = mServer.nextPendingConnection();
    if(socket)
    {
        mPythonProcess = new PythonProcessControl(socket);
        HilecCore& hilec = HilecCore::instance();
    }
}
