// OFFIS Automation Framework
// Copyright (C) 2013-2016 OFFIS e.V.
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

#include "PythonDebugger.h"
#include "PythonTypeConverter.h"

#undef slots
#include "Python.h"
#include "PythonPlugin.h"

PythonInterpreter::PythonInterpreter(const QString &configDir)
    : mDebugger(new PythonDebugger())
{
    mConfigDir = configDir;
	PyImport_AppendInittab("offisio", initIoModule);
}

PythonInterpreter::~PythonInterpreter()
{
    mDebugger->step(PythonDebugger::Quit);
    wait();
    delete mDebugger;

	Py_Finalize();
}

void PythonInterpreter::start(const QString &filename, const QString &baseDir)
{
	if (filename.isEmpty())
        return;

    mBaseDir = baseDir;
    mFilename = filename;
    QThread::start();
}

void PythonInterpreter::run()
{
    qDebug() << "Python: Executing" << mFilename;
    mDebugger->step();
    QString last = QDir::currentPath();
    QDir::setCurrent(mBaseDir);

	wchar_t path[2048], pName[2048];
	memset(path, 0, sizeof(path));
	memset(pName, 0, sizeof(pName));
	wchar_t* pNamePtr = pName;
	mFilename.toWCharArray(pName);

	Py_SetProgramName(pName);

	QString(
		mBaseDir + ";"
		+ mConfigDir + "/python;"
		+ mConfigDir + "/python/site-packages;"
		// Fall back to the system path
		+ QString::fromWCharArray(Py_GetPath()) + ";"
		+ QString::fromWCharArray(Py_GetPrefix()) + "/Lib/site-packages;"
	).toWCharArray(path);

    Py_SetPath(path);
	mDebugger->initialize();

	Py_Initialize();
	PySys_SetArgvEx(1, &pNamePtr, 0);
    PyEval_SetTrace(PythonDebugger_Trace, 0);
    runFile(mConfigDir + "/python/offis/init.py");
    runFile(mFilename);
    UserRequestManager::instance()->abortAll();

	Py_Finalize();
    mDebugger->deinitialize();
    QDir::setCurrent(last);
    qDebug() << "Python: Execution finished" << mFilename;
    HilecCore::instance().scriptPaused(QString(), -1);
}

void PythonInterpreter::runFile(const QString &filename)
{
    FILE* file = _Py_fopen(qPrintable(filename), "r");

	if(!file)
        return;

	PyRun_SimpleFile(file, qPrintable(filename));
    fclose(file);
}

void PythonInterpreter::addBreakpoint(const QString &file, int line)
{
    mDebugger->addBreakpoint(file, line);
}

void PythonInterpreter::removeBreakpoint(const QString &file, int line)
{
    mDebugger->removeBreakpoint(file, line);
}

QList<QPair<QString, int> > PythonInterpreter::breakpoints() const
{
    return mDebugger->breakpoints();
}

void PythonInterpreter::resume()
{
    mDebugger->step(PythonDebugger::Resume);
}

void PythonInterpreter::stepInto()
{
    mDebugger->step(PythonDebugger::StepInto);
}

void PythonInterpreter::stepOver()
{
    mDebugger->step(PythonDebugger::StepOver);
}

void PythonInterpreter::stepReturn()
{
    mDebugger->step(PythonDebugger::StepReturn);
}

void PythonInterpreter::quit()
{
    UserRequestManager::instance()->abortAll();
    mDebugger->step(PythonDebugger::Quit);
    PyErr_SetInterrupt();
}

QAbstractItemModel* PythonInterpreter::debugVars(int frameDepth)
{
    return mDebugger->variables(frameDepth);
}

QList<TraceLine> PythonInterpreter::getStackTrace()
{
    return mDebugger->callStack();
}
