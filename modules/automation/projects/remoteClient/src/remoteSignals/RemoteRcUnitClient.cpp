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


#include "RemoteRcUnitClient.h"
#include <lolecs/RcExceptions.h>

RemoteRcUnitClient::RemoteRcUnitClient(QIODevice *writeDevice, QIODevice *readDevice, bool initialize)
    : RemoteRcUnitClientBase(writeDevice, readDevice, initialize)
{
    mNextId = 0;
}

QVariant RemoteRcUnitClient::callMethod(const QByteArray &unit, const QByteArray &name, const QVariantList &params, unsigned long timeout)
{
    QMutexLocker lock(&mMutex);
    uint id = mNextId++;
    RemoteCall call;
    mRemoteCalls[id] = &call;
    RemoteRcUnitClientBase::callMethod(id, unit, name, params);
    bool hasTimeout = !call.waiter.wait(&mMutex, timeout);
    mRemoteCalls.remove(id);
    if(hasTimeout)
        throw RcError(QString("Timeout calling %1:%2").arg(QString(unit), QString(name)));
    if(call.isError)
        throw RcError(QString("Error calling %1:%2: %3").arg(unit, name, call.returnValue.toString()));
    return call.returnValue;
}

QList<RcUnitHelp> RemoteRcUnitClient::listUnits(ulong timeout)
{
    QMutexLocker lock(&mMutex);
    RemoteRcUnitClientBase::listUnits();
    if(!mListUnitWait.wait(&mMutex, timeout))
        throw RcError(QString("Timeout retrieving remote unit list"));
    return mLastUnitList;
}

void RemoteRcUnitClient::onUnitList(const QList<RcUnitHelp> &list)
{
    QMutexLocker lock(&mMutex);
    mLastUnitList = list;
    mListUnitWait.wakeAll();
}

void RemoteRcUnitClient::insertCallResponse(uint id, QVariant value, bool isError)
{
    QMutexLocker lock(&mMutex);
    RemoteCall* call = mRemoteCalls.value(id, 0);
    if(call)
    {
        call->isError = isError;
        call->returnValue = value;
        call->waiter.wakeAll();
    }
}

