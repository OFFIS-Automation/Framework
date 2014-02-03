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

#include "RemoteLolec.h"
#include <QDebug>
#include <QStringList>
#include <QtEndian>
#include <lolecs/RcExceptions.h>
#include "RemoteRcUnit.h"

RemoteLolec::RemoteLolec(const QString &name, const QString &host, int port) :
    mName(name),
    mHost(host),
    mPort(port),
    mTimeout(10000)
{
    connect(&mSocket, SIGNAL(connected()), SLOT(onConnected()));
    connect(&mSocket, SIGNAL(disconnected()), SLOT(onDisconnected()));
    connect(&mSocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(onError(QAbstractSocket::SocketError)));
    connect(&mSocket, SIGNAL(readyRead()), SLOT(onReadyRead()));

    connect(this, SIGNAL(sendMessage(QByteArray)), SLOT(onSendMessage(QByteArray)));
    mNextCallId = 0;
}
void RemoteLolec::startConnect()
{
    if(mSocket.state() == QAbstractSocket::UnconnectedState)
        mSocket.connectToHost(mHost, mPort);
}

RemoteLolec::~RemoteLolec()
{
    mSocket.disconnect(this);
    mSocket.close();
}

void RemoteLolec::onConnected()
{
    mReadSize = 0;
    getLolecs();
}

void RemoteLolec::onDisconnected()
{
    mDataMutex.lock();
    QString error = tr("Connection to remote server lost");
    QList<int> ids = mCalls.keys();
    foreach(int id, ids)
    {
        mCalls[id]->error = error;
        mCalls[id]->hasError = true;
        mCalls[id]->waiter.wakeAll();
    }
    mDataMutex.unlock();

    QList<QString> old = mLolecs.keys();
    mLolecs.clear();
    emit lolecsUpdated(mName, old);
    mSocket.connectToHost(mHost, mPort);
}

void RemoteLolec::onError(QAbstractSocket::SocketError /*socketError*/)
{
    mSocket.connectToHost(mHost, mPort);
}

void RemoteLolec::getLolecs()
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << QString("list");
    sendMessage(data);
}

void RemoteLolec::onSendMessage(const QByteArray &msgData)
{
    QMutexLocker lock(&mMutex);
    QByteArray sizeData(4,0);
    qToLittleEndian<int>(msgData.size(), (uchar*)sizeData.data());

    mSocket.write(sizeData);
    mSocket.write(msgData);
}

void RemoteLolec::onReadyRead()
{
    while(true)
    {
        if(mReadSize)
        {
            if(mSocket.bytesAvailable() < mReadSize)
                return;
            mReadSize = 0;
            QDataStream stream(&mSocket);
            QString method;
            stream >> method;
            dispatch(method, stream);
        }
        else if(mSocket.bytesAvailable() >= 4)
        {
            QByteArray sizeArray = mSocket.read(4);
            mReadSize = qFromLittleEndian<int>((uchar*)sizeArray.data());
        }
        else
            return;
    }
}

void RemoteLolec::dispatch(const QString &method, QDataStream &stream)
{
    if(method == "list")
    {
        QVariant data;
        stream >> data;
        decodeListResponse(data);
    }

    else if(method == "call")
    {
        int id;
        QVariant answer;
        stream >> id >> answer;
        QMutexLocker lock(&mDataMutex);
        if(mCalls.contains(id))
        {
            RemoteCall* call = mCalls[id];
            call->response = answer;
            call->hasError = false;
            call->waiter.wakeAll();
        }
    }
    else if(method == "callError")
    {
        int id;
        QString error;
        stream >> id >> error;
        QMutexLocker lock(&mDataMutex);
        if(mCalls.contains(id))
        {
            RemoteCall* call = mCalls[id];
            call->error = error;
            call->hasError = true;
            call->waiter.wakeAll();
        }
    }
    else if(method.left(2) == "tc")
    {
        int id;
        stream >> id;
        QMutexLocker lock(&mDataMutex);
        if(mCalls.contains(id))
            mCalls[id]->waiter.wakeAll();
    }
}

void RemoteLolec::decodeListResponse(const QVariant &data)
{

    QList<QString> old = mLolecs.keys();
    mLolecs.clear();
    QListIterator<QVariant> list = data.toList();
    while(list.hasNext())
    {
        QString name = list.next().toString();
        RcUnitHelp& help = mLolecs[name];
        help.desc = list.next().toString();
        QVariantList structs = list.next().toList();
        QVariantList methods = list.next().toList();
        help.constants = list.next().toMap();
        QVariantList tcMethods = list.next().toList();
        foreach(const QVariant& structData, structs)
        {
            QStringList structList = structData.toStringList();
            RcUnitHelp::Struct structItem;
            structItem.name = structList.takeFirst();
            structItem.members = structList;
            help.structs << structItem;
        }
        foreach(const QVariant& methodData, methods)
        {
            QStringList methodList = methodData.toStringList();
            if(methodList.size() != 4) continue;
            RcUnitHelp::Method methodItem;
            methodItem.name = methodList[0];
            methodItem.sig = methodList[1];
            methodItem.shortDesc = methodList[2];
            methodItem.longDesc = methodList[3];
            help.methods << methodItem;
        }
        foreach(const QVariant& methodData, tcMethods)
        {
            QVariantList methodList = methodData.toList();
            if(methodList.size() != 4) continue;
            RcUnitHelp::TcJostick methodItem;
            methodItem.name = methodList[0].toString();
            methodItem.deadMansButton = methodList[1].toInt();
            methodItem.sensitivity = methodList[2].toDouble();
            methodItem.axeNames = methodList[3].toStringList();
            for(int i=0; i< methodItem.axeNames.size(); i++)
                methodItem.inverts.append(false);
            help.tcJoysticks << methodItem;
        }
    }
    emit lolecsUpdated(mName, old);
}

QList<RcUnitBase *> RemoteLolec::units()
{
    QList<RcUnitBase*> units;
    foreach(QString name, mLolecs.keys())
    {
        const RcUnitHelp& help = mLolecs.value(name);
        units << new RemoteRcUnit(name, help, *this);
    }
    return units;
}

QVariant RemoteLolec::callMethod(const QByteArray& unit, const QByteArray &name, const QList<QVariant> &params)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    QMutexLocker lock(&mDataMutex);
    int id = ++mNextCallId;
    stream << QString("call");
    stream << unit;
    stream << name;
    stream << id;
    stream << params;
    RemoteCall call;
    mCalls[id] = &call;
    emit sendMessage(data);
    bool timeout = !call.waiter.wait(&mDataMutex, mTimeout);
    mCalls.take(id);
    if(timeout)
        throw RcError("Timeout getting response from remote server");
    if(call.hasError)
        throw RcError(call.error);
    return call.response;
}

double RemoteLolec::timout()
{
    return mTimeout / 1000.0;
}

void RemoteLolec::setTimeout(double seconds)
{
    mTimeout= seconds * 1000;
}

void RemoteLolec::updateSensitivity(const QString &unit, const QString &tcName, double sensitivity, const QList<bool> &inverts)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    QMutexLocker lock(&mDataMutex);
    int id = ++mNextCallId;
    stream << QString("tcSens");
    stream << id;
    stream << unit;
    stream << tcName;
    stream << sensitivity;
    stream << inverts;
    emit sendMessage(data);
}

void RemoteLolec::connectGamepad(const QString &unitName)
{
    updateGamepad("tcEnable", unitName);
}

void RemoteLolec::disconnectGamepad(const QString &unitName)
{
    updateGamepad("tcDisable", unitName);
}

void RemoteLolec::updateGamepad(const QString &cmd, const QString &unitName)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    QMutexLocker lock(&mDataMutex);
    int id = ++mNextCallId;
    stream << cmd;
    stream << id;
    stream << unitName;
    emit sendMessage(data);

}

void RemoteLolec::handleGamepadData(const QMap<int, double> &tcData)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    QMutexLocker lock(&mDataMutex);
    int id = ++mNextCallId;
    stream << QString("tcData");
    stream << id;
    stream << tcData;
    RemoteCall call;
    mCalls[id] = &call;
    emit sendMessage(data);
    bool timeout = !call.waiter.wait(&mDataMutex, 1000);
    mCalls.take(id);
    if(timeout)
        qCritical() << tr("RemoteGamepadUpdate: Timeout getting response from remote server");
}

void RemoteLolec::setGamepadButton(int buttonId, bool pressed)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    QMutexLocker lock(&mDataMutex);
    int id = ++mNextCallId;
    stream << QString("tcButton");
    stream << id;
    stream << buttonId;
    stream << pressed;
    RemoteCall call;
    mCalls[id] = &call;
    emit sendMessage(data);
    bool timeout = !call.waiter.wait(&mDataMutex, 1000);
    mCalls.take(id);
    if(timeout)
        qCritical() << tr("RemoteGamepadSetButton: Timeout getting response from remote server");

}

