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
#include <rc/RcExceptions.h>
#include "RemoteRcUnit.h"

RemoteRcUnits::RemoteRcUnits(const QString &name, const QString &host, int port, double timeout) :
    mName(name),
    mHost(host),
    mPort(port),
    mTimeout(timeout)
{
}

void RemoteRcUnits::startConnect()
{
    QMutexLocker lock(&mMutex);
    start();
    mStartupWait.wait(&mMutex);
}

RemoteRcUnits::~RemoteRcUnits()
{
    quit();
    wait();

}

void RemoteRcUnits::onConnected()
{
    QMutexLocker lock(&mMutex);
    QList<RcUnitHelp> list = mClient->listUnits();
    QList<QString> old = mLolecs.keys();
    mLolecs.clear();
    foreach(const RcUnitHelp& help, list)
        mLolecs.insert(help.unitName, help);
    emit unitsUpdated(mName, old);
}

void RemoteRcUnits::onDisconnected()
{
    QMutexLocker lock(&mMutex);
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
    mSocket->connectToHost(mHost, mPort);
}

void RemoteRcUnits::onError(QAbstractSocket::SocketError /*socketError*/)
{
    mSocket->connectToHost(mHost, mPort);
}

void RemoteRcUnits::run()
{
    {
        QMutexLocker lock(&mMutex);
        mSocket = new QTcpSocket();
        connect(mSocket, SIGNAL(connected()), SLOT(onConnected()));
        connect(mSocket, SIGNAL(disconnected()), SLOT(onDisconnected()), Qt::DirectConnection);
        connect(mSocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(onError(QAbstractSocket::SocketError)), Qt::DirectConnection);
        mClient = new RemoteRcUnitClient(mSocket, mSocket);
        mClient->setDefaultTimeout(mTimeout * 1000);
        if(mSocket->state() == QAbstractSocket::UnconnectedState)
            mSocket->connectToHost(mHost, mPort);
        mStartupWait.wakeAll();
    }
    exec();
    QMutexLocker lock(&mMutex);
    mSocket->disconnect(this);
    mSocket->disconnectFromHost();
    QList<QString> old = mLolecs.keys();
    mLolecs.clear();
    emit unitsUpdated(mName, old);
    delete mClient;
    delete mSocket;
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
    return mTimeout;
}


void RemoteRcUnits::setTimeout(double seconds)
{
    QMutexLocker lock(&mMutex);
    if(mClient)
        mClient->setDefaultTimeout(seconds * 1000);
    mTimeout = seconds;
}

