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

#ifndef PYTHONINTERPRETER_H
#define PYTHONINTERPRETER_H

#include <QThread>
#include <QMutex>
#include <QStringList>
#include <QAbstractItemModel>
#include <core/ScriptException.h>
#include <QLocalServer>
#include "PythonProcessControl.h"


class PythonInterpreter : public QObject
{
    Q_OBJECT
public:
    PythonInterpreter(const QString& configDir);
    virtual ~PythonInterpreter();
    void start(const QString& filename, const QString& baseDir);
    void addBreakpoint(const QString& file, int line);
    void removeBreakpoint(const QString& file, int line);
    QList<QPair<QString, int> >  breakpoints() const;
    bool wait(int timeout);
    bool isRunning() { return true;  }//TODO

    bool userInput (int uid, int buttonId, const QList<QVariant> &data);
    void resume();
    void stepInto();
    void stepOver();
    void stepReturn();
    void quit();
    QAbstractItemModel* debugVars(int frameDepth);
    QList<TraceLine> getStackTrace();
private slots:
    void connectScript();
protected:
    QLocalServer mServer;
    QList<QPair<QString, int> > mBreakpoints;
    QMutex mMutex;
    QString mConfigDir;
    QString mBaseDir;
    PythonProcessControl* mPythonProcess;
};

#endif // PYTHONINTERPRETER_H
