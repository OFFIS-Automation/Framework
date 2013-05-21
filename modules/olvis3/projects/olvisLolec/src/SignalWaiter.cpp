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

#include "SignalWaiter.h"

SignalWaiter::SignalWaiter(QObject *parent, const char* asignal) :
    QObject(parent) , mFinished(false)
{
    connect(parent, asignal, SLOT(wakeup()));
}

bool SignalWaiter::wait(double timeoutSec)
{
    QMutexLocker lock(&mMutex);
    if(mFinished) return true;
    ulong timeout = timeoutSec * 1000;
    mWait.wait(&mMutex, timeout);
    return mFinished;
}

void SignalWaiter::wakeup()
{
    QMutexLocker lock(&mMutex);
    mFinished = true;
    parent()->disconnect(this);
    mWait.wakeAll();
}
