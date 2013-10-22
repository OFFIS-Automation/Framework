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


#include "PythonProcessSingleton.h"
#include <QCoreApplication>

#include <lolecs/Pose2d.h>

PythonProcessServer *PythonProcessSingleton::serverInstance()
{
    PythonProcessSingleton& inst = instance();
    QMutexLocker lock(&inst.mMutex);
    return inst.mServer;
}

PythonProcessSingleton& PythonProcessSingleton::instance()
{
    static PythonProcessSingleton inst;
    return inst;
}

bool PythonProcessSingleton::connectToServer(const QString &socketName)
{
    QMutexLocker lock(&mMutex);
    mSocketName = "OffisFrameworkPythonSocket";
    mConnectWait.wakeAll();
    mStartupWait.wait(&mMutex);
    return mConnected;
}

void PythonProcessSingleton::run()
{
    QLocalSocket socket;
    PythonProcessServer server(&socket, &socket);
    {
        QMutexLocker lock(&mMutex);
        mServer = &server;
        mStartupWait.wakeAll();
        mConnectWait.wait(&mMutex);
        socket.connectToServer(mSocketName);
        mConnected = socket.waitForConnected(500);
        server.initialize();
        mStartupWait.wakeAll();
    }
    QThread::exec();
}

PythonProcessSingleton::PythonProcessSingleton()
{
    qRegisterMetaType<Pose2d>("Pose2d");
    qRegisterMetaTypeStreamOperators<Pose2d>("Pose2d");
    QMutexLocker lock(&mMutex);
    start();
    mStartupWait.wait(&mMutex);
}
