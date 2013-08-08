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


#include "PythonProcessControl.h"
#include "HilecCore.h"
#include "RcUnits.h"

class RcUnitExecuter : public QRunnable
{
public:
    RcUnitExecuter(int id, const QByteArray& unit, const QByteArray& method, const QVariantList& params, PythonProcessControl& processcontrol)
        : mId(id), mUnit(unit), mMethod(method), mParams(params), mProcessControl(processcontrol)
    {}
private:
    void run() {
        try
        {
            QVariant result =  RcUnits::instance()->call(mUnit, mMethod, mParams);
            mProcessControl.rcUnitResult(mId, result);
        }
        catch(const std::runtime_error& err)
        {
            mProcessControl.rcUnitException(mId, err.what());
        }
    }
    int mId;
    QByteArray mUnit;
    QByteArray mMethod;
    QVariantList mParams;
    PythonProcessControl& mProcessControl;
};

PythonProcessControl::PythonProcessControl(QLocalSocket *socket)
    : PythonProcessClient(*socket, *socket, false)
{
    HilecCore* hilec = &HilecCore::instance();
    connect(this, SIGNAL(appendInfo(QString)), hilec, SIGNAL(infoAppended(QString)));
    connect(this, SIGNAL(clearInfo()), hilec, SIGNAL(infoCleared()));
    connect(this, SIGNAL(createInfoPanel(int,QString,QStringList)), hilec, SIGNAL(newInfoPanel(int,QString,QStringList)));
    connect(this, SIGNAL(removeInfoPanel(int)), hilec, SIGNAL(infoPanelRemoved(int)));
    connect(this, SIGNAL(updateInfoPanel(int,QStringList)), hilec, SIGNAL(infoPanelUpdated(int,QStringList)));
    connect(this, SIGNAL(createProgress(int,QString,int)), hilec, SIGNAL(newProgressBar(int,QString,int)));
    connect(this, SIGNAL(removeProgress(int)), hilec, SIGNAL(progressBarRemoved(int)));
    connect(this, SIGNAL(updateProgress(int,int)), hilec, SIGNAL(progressBarUpdated(int,int)));
    connect(this, SIGNAL(printText(QString)), hilec, SIGNAL(newTextStr(QString)));
    connect(this, SIGNAL(printError(QString)), hilec, SIGNAL(newErrorStr(QString)));
    connect(this, SIGNAL(raiseException(ScriptException)), hilec , SIGNAL(scriptException(ScriptException)));
    connect(this, SIGNAL(scriptPaused(QString,int)), hilec, SIGNAL(breakpointHit(QString,int)));
    connect(this, SIGNAL(userRequest(UserRequest)), hilec, SIGNAL(userInputRequested(UserRequest)));
    connect(this, SIGNAL(userRequestAbort(int)), hilec, SIGNAL(userInputAborted(int)));
    connect(this, SIGNAL(callRcUnit(int,QByteArray,QByteArray,QVariantList)), SLOT(executeRcUnit(int,QByteArray,QByteArray,QVariantList)));
    connect(this, SIGNAL(updateCallStack(QList<TraceLine>)), SLOT(receiveCallStack(QList<TraceLine>)), Qt::DirectConnection);
    initialize();
}

QList<TraceLine> PythonProcessControl::getCallStack()
{
    QMutexLocker lock(&mMutex);
    requestCallStack();
    if(mWait.wait(&mMutex, 1000))
        return mLastCallStack;
    return QList<TraceLine>();
}

void PythonProcessControl::executeRcUnit(int id, const QByteArray &unit, const QByteArray &method, const QVariantList &params)
{
    mThreadPool.start(new RcUnitExecuter(id, unit, method, params, *this));
}

void PythonProcessControl::receiveCallStack(const QList<TraceLine> &trace)
{
    QMutexLocker lock(&mMutex);
    mLastCallStack = trace;
    mWait.wakeAll();
}
