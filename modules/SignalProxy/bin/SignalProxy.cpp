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

SignalProxy::SignalProxy(QIODevice &readDevice, QIODevice &writeDevice) :
    mReadDevice(readDevice),
    mWriteDevice(writeDevice)
{
    mReadSize = 0;
}

void SignalProxy::transmitSignal(const QByteArray &msgData)
{
    QByteArray sizeData(4,0);
    qToLittleEndian<int>(msgData.size(), (uchar*)sizeData.data());
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
void SignalProxy::handleError(const QUuid id)
{
    qCritical() << "the signal with the id " << id << " was not found!";
    qFatal("A signal id was not found, this is a critical failure! Maybe Client and Server are out of sync?");
}
