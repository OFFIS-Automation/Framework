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
// along with this program.  If not, see <http:#www.gnu.org/licenses/>.

#include "SignalProxy.h"
#include <QtEndian>
#include <QDebug>

SignalProxy::SignalProxy(quint64 gid1, quint64 gid2, QIODevice &readDevice, QIODevice &writeDevice) :
    mGlobalId1(gid1),
    mGlobalId2(gid2),
    mReadDevice(readDevice),
    mWriteDevice(writeDevice)
{
    mReadSize = 0;
    connect(&mReadDevice, SIGNAL(readyRead()), SLOT(onReadyRead()), Qt::DirectConnection);
}

void SignalProxy::transmitSignal(const QByteArray &msgData)
{
    QByteArray sizeData(4,0);
    qToLittleEndian<int>(msgData.size(), (uchar*)sizeData.data());
    QMutexLocker lock(&mMutex);
    mWriteDevice.write(sizeData);
    mWriteDevice.write(msgData);
}
void SignalProxy::onReadyRead()
{
    while(true)
    {
        qint64 bytes = mReadDevice.bytesAvailable();
        if(mReadSize)
        {
            if(bytes < mReadSize)
                return;
            QByteArray data = mReadDevice.read(mReadSize);
            processRemoteInputs(data);
            mReadSize = 0;
        }
        else if(bytes >= 4)
        {
            QByteArray sizeArray = mReadDevice.read(4);
            mReadSize = qFromLittleEndian<int>((uchar*)sizeArray.data());
        }
        else
            return;
    }
}
void SignalProxy::handleError(int id)
{
    qCritical() << "the signal with the id " << id << " was not found!";
    qFatal("A signal id was not found, this is a critical failure! Maybe Client and Server are out of sync?");
}

void SignalProxy::checkId(quint64 id1, quint64 id2)
{
    if(id1 != gid1() || id2 != gid2())
        qFatal("The global id of the sender did not match the id of the receiver. Client and Server  are out of sync.");
}
