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

#include "PythonDebugger.h"
#include <QDebug>
#include <QFileInfo>

#include "PythonTypeConverter.h"
#include "CallStackDecoder.h"

#undef slots
#include "Python.h"
#include <frameobject.h>

extern "C" int PythonDebugger_Trace(PyObject*, PyFrameObject* frame, int what, PyObject* arg)
{
    PythonDebugger* dbg = PythonDebugger::instance();
    if(dbg)
        return dbg->trace(frame, what, arg);
    return 1;
}

PythonDebugger* PythonDebugger::sInstance = 0;

PythonDebugger::PythonDebugger()
{
    sInstance = this;
    mCallStackDecoder = new CallStackDecoder;

}

PythonDebugger::~PythonDebugger()
{
    sInstance = 0;
    delete mCallStackDecoder;
}

void PythonDebugger::initialize()
{
}

void PythonDebugger::deinitialize()
{
    mCallStackDecoder->reset();
}

int PythonDebugger::trace(PyFrameObject *frame, int what, PyObject *)
{
    if(mStepMode == Quit)
    {
        PyErr_SetString(PyExc_SystemError, "Quit");
        mStepMode = Resume;
        return 1;
    }
    else if(mStepMode == StepOver && what == PyTrace_CALL)
    {
        mBreakDepth++;
        return 0;
    }
    else if(mStepMode == StepOver && what == PyTrace_RETURN)
    {
        mBreakDepth--;
        return 0;
    }
    if(what != PyTrace_LINE)
        return 0;
    bool pause = false;
    if(mStepMode == StepInto)
        pause = true;
    else if(mStepMode == StepOver && mBreakDepth <= 0)
        pause = true;
    int line = frame->f_lineno;
    if(pause || (!mBreakpoints.empty() && mBreakpoints.contains(line)))
    {
        QString filename = getFilename(frame);
        QMutexLocker lock(&mMutex);
        if(pause || mBreakpoints[line].contains(filename))
        {
            mCallStackDecoder->decode(frame);
            HilecCore::instance().scriptPaused(filename, line);
            mWait.wait(&mMutex);
            mCallStackDecoder->prepareResume();
            PyFrame_LocalsToFast(frame, 0);
            if(mStepMode == Quit)
            {
                PyErr_SetString(PyExc_SystemError, "Quit");
                mStepMode = Resume;
                return 1;
            }
            if(mStepMode == StepReturn)
            {
                mBreakDepth = 1;
                mStepMode = StepOver;
            } else
            {
                mBreakDepth = 0;
            }
        }
    }
    return 0;
}

QString PythonDebugger::getFilename(_frame *frame)
{
    QString filename = PythonTypeConverter::toString(frame->f_code->co_filename);
    return QFileInfo(filename).canonicalFilePath();
}


void PythonDebugger::addBreakpoint(const QString &file, int line)
{
    QMutexLocker lock(&mMutex);
    mBreakpoints[line].insert(QFileInfo(file).canonicalFilePath());
}

void PythonDebugger::removeBreakpoint(const QString &file, int line)
{
    QMutexLocker lock(&mMutex);
    mBreakpoints[line].remove(QFileInfo(file).canonicalFilePath());
    if(mBreakpoints[line].isEmpty())
        mBreakpoints.remove(line);
}

void PythonDebugger::step(StepMode stepMode)
{
    QMutexLocker lock(&mMutex);
    mStepMode = stepMode;
    mWait.wakeAll();
}

QAbstractItemModel* PythonDebugger::variables(int frameDepth)
{
    QMutexLocker lock(&mMutex);
    return mCallStackDecoder->locals(frameDepth);
}

QList<TraceLine> PythonDebugger::callStack()
{
    QMutexLocker lock(&mMutex);
    return mCallStackDecoder->trace();
}
