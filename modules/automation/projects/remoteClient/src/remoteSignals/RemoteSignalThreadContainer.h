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


#ifndef REMOTERCUNITCLIENTTHREADCONTAINER_H
#define REMOTERCUNITCLIENTTHREADCONTAINER_H

#include <QThread>
#include "RemoteRcUnitClient.h"

template <class T> class RemoteSignalThreadContainer : public QThread
{
public:
    explicit RemoteSignalThreadContainer() :
        mObject(0)
    {
    }
    void start(QIODevice* readDevice, QIODevice* writeDevice = 0, bool initialize = true)
    {
        QMutexLocker lock(&mMutex);
        mReadDevice = readDevice;
        mWriteDevice = writeDevice;
        mInitialize = initialize;
        QThread::start();
        mStartupWait.wait(&mMutex);
    }
    T* object()
    {
        QMutexLocker lock(&mMutex);
        return mObject;
    }
private:
    void run()
    {
        {
            QMutexLocker lock(&mMutex);
            mObject = new T(mReadDevice, mWriteDevice, mInitialize);
            mStartupWait.wakeAll();
        }
        QThread::exec();
        QMutexLocker lock(&mMutex);
        delete mObject;
        mStartupWait.wakeAll();

    }
    QIODevice* mReadDevice, *mWriteDevice;
    QMutex mMutex;
    QWaitCondition mStartupWait;
    bool mInitialize;
    T* mObject;
    
};

#endif // REMOTERCUNITCLIENTTHREADCONTAINER_H
