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

#include "RemoteRcUnits.h"
#include <QDebug>
#include <QStringList>
#include <QtEndian>
#include "remoteSignals/RemoteRcUnitClient.h"
#include <lolecs/RcExceptions.h>
#include "RemoteRcUnit.h"

RemoteRcUnits::RemoteRcUnits(const QString &name, const QString &host, int port) :
    mName(name),
    mHost(host),
    mPort(port)
{
    mClient = new RemoteRcUnitClient(&mSocket);
    connect(&mSocket, SIGNAL(connected()), SLOT(onConnected()));
    connect(&mSocket, SIGNAL(disconnected()), SLOT(onDisconnected()));
    connect(&mSocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(onError(QAbstractSocket::SocketError)));
}
void RemoteRcUnits::startConnect()
{
    if(mSocket.state() == QAbstractSocket::UnconnectedState)
        mSocket.connectToHost(mHost, mPort);
}

RemoteRcUnits::~RemoteRcUnits()
{
    mSocket.disconnect(this);
    mSocket.close();
    delete mClient;
}

void RemoteRcUnits::onConnected()
{
    QList<RcUnitHelp> list = mClient->listUnits();
    QList<QString> old = mLolecs.keys();
    mLolecs.clear();
    foreach(const RcUnitHelp& help, list)
        mLolecs.insert(help.unitName, help);
    emit unitsUpdated(mName, old);
}

void RemoteRcUnits::onDisconnected()
{
/*
    QString error = tr("Connection to remote server lost");
/*
    QList<int> ids = mCalls.keys();
    foreach(int id, ids)
    {
        mCalls[id]->error = error;
        mCalls[id]->hasError = true;
        mCalls[id]->waiter.wakeAll();
    }
    mDataMutex.unlock();
*/
    QList<QString> old = mLolecs.keys();
    mLolecs.clear();
    emit unitsUpdated(mName, old);
    mSocket.connectToHost(mHost, mPort);
}

void RemoteRcUnits::onError(QAbstractSocket::SocketError /*socketError*/)
{
    mSocket.connectToHost(mHost, mPort);
}






QList<RcUnitBase *> RemoteRcUnits::units()
{
    QList<RcUnitBase*> units;
    foreach(QString name, mLolecs.keys())
    {
        RcUnitHelp help = mLolecs.value(name);
        help.server = mName;
        units << new RemoteRcUnit(help, *mClient);
    }
    return units;
}


double RemoteRcUnits::timout()
{
    return mClient->defaultTimeout() / 1000.0;
}

void RemoteRcUnits::setTimeout(double seconds)
{
    mClient->setDefaultTimeout(seconds * 1000);
}

