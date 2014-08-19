// OFFIS Automation Framework
// Copyright (C) 2013-2014 OFFIS e.V.
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

#ifndef PYTHONDEBUGGER_H
#define PYTHONDEBUGGER_H

#include <QMap>
#include <QSet>
#include <QMutex>
#include <QWaitCondition>
#include <QAbstractItemModel>
#include "HilecCore.h"

class CallStackDecoder;

struct _frame;
struct _object;

class PythonDebugger
{
public:
    enum StepMode {
        Quit,
        StepInto,
        StepOver,
        StepReturn,
        Resume
    };

    PythonDebugger();
    virtual ~PythonDebugger();
    static PythonDebugger* instance() { return sInstance; }
    void initialize();
    void deinitialize();
    int trace(_frame* frame, int what, _object* arg);
    void addBreakpoint(const QString& file, int line);
    void removeBreakpoint(const QString& file, int line);
    QList<QPair<QString, int> > breakpoints() const;
    void step(StepMode stepMode = Resume);
    QAbstractItemModel* variables(int frameDepth);
    QList<TraceLine> callStack();
protected:
    QString getFilename(_frame* frame);
    static PythonDebugger* sInstance;
    typedef QSet<QString> StringSet;
    QMap<int, StringSet> mBreakpoints;
    QMutex mMutex;
    QWaitCondition mWait;
    StepMode mStepMode;
    int mBreakDepth;
    CallStackDecoder* mCallStackDecoder;
};

extern "C" int PythonDebugger_Trace(_object* obj, _frame* frame, int what, _object* arg);

#endif // PYTHONDEBUGGER_H
