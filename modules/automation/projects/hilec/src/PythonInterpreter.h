// OFFIS Automation Framework
// Copyright (C) 2013-2017 OFFIS e.V.
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

#ifndef PYTHONINTERPRETER_H
#define PYTHONINTERPRETER_H

#include <QThread>
#include <QMutex>
#include <QStringList>
#include <QAbstractItemModel>
#include <core/ScriptException.h>

class PythonDebugger;

class PythonInterpreter : public QThread
{
public:
    PythonInterpreter(const QString& configDir);
    virtual ~PythonInterpreter();
    void start(const QString& filename, const QString& baseDir);
    void addBreakpoint(const QString& file, int line);
    void removeBreakpoint(const QString& file, int line);
    QList<QPair<QString, int> >  breakpoints() const;
    void resume();
    void stepInto();
    void stepOver();
    void stepReturn();
    void quit();
    QAbstractItemModel* debugVars(int frameDepth);
    QList<TraceLine> getStackTrace();
protected:
    virtual void run();
    void runFile(const QString& filename);
    QString mFilename;
    QStringList mFilesToParse;
    QMutex mMutex;
    QString mConfigDir;
    QString mBaseDir;
    PythonDebugger* mDebugger;
};

#endif // PYTHONINTERPRETER_H
