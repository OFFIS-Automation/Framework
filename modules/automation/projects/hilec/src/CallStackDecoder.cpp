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

#include "CallStackDecoder.h"
#include "PythonTypeConverter.h"

#include <QFileInfo>

#undef slots
#include "Python.h"
#include <frameobject.h>

CallStackDecoder::CallStackDecoder()
{
    mCurrentFrame = 0;
}

void CallStackDecoder::reset()
{
    prepareResume();
    QMutexLocker lock(&mMutex);
    mVars.setVariables(0);
}

void CallStackDecoder::prepareResume()
{
    QMutexLocker lock(&mMutex);
    mTrace.clear();
    mCurrentFrame = 0;
    foreach(_frame* frame, mConvertedLocals)
        PyFrame_LocalsToFast(frame, 0);
    mConvertedLocals.clear();
}

void CallStackDecoder::decode(_frame *frame)
{
    QMutexLocker lock(&mMutex);
    mTrace.clear();
    mCurrentFrame = frame;
    while(frame)
    {
        TraceLine traceLine;
        traceLine.line = frame->f_lineno;
        traceLine.file = PythonTypeConverter::toString(frame->f_code->co_filename, true);
        traceLine.file = QFileInfo(traceLine.file).canonicalFilePath();
        traceLine.method = PythonTypeConverter::toString(frame->f_code->co_name);
        if(traceLine.method.isEmpty())
                traceLine.method = "lambda";
        PyObject* name = PyBytes_FromString("__name__");
        PyObject* modName = PyDict_GetItem(frame->f_globals, name);
        Py_XDECREF(name);
        traceLine.module = PythonTypeConverter::toString(modName, true);
        mTrace << traceLine;
        frame = frame->f_back;
    }
    // set the locals
    locals(0);
}

QList<TraceLine> CallStackDecoder::trace()
{
   QMutexLocker lock(&mMutex);
   return mTrace;
}

QAbstractItemModel * CallStackDecoder::locals(int frameDepth)
{
    _frame* frame = mCurrentFrame;
    for(int i=0; i<frameDepth; i++)
    {
        if(!frame)
            break;
        frame = frame->f_back;
    }
    if(frame)
    {
        if(!mConvertedLocals.contains(frame))
        {
            PyFrame_FastToLocals(frame);
            mConvertedLocals.insert(frame);
        }
        mVars.setVariables(frame->f_locals);
    }
    else
        mVars.setVariables(0);
    return &mVars;
}

