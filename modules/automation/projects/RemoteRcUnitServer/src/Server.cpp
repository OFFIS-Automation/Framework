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

#include "Server.h"
#include <QSettings>
#include <QTcpSocket>
#include <QtEndian>
#include <QStringList>
#include <QDebug>

Server::Server() : mTcpServer(this), mSocket(0)
{
    connect(&mTcpServer, SIGNAL(newConnection()), SLOT(onConnect()));
}

Server::~Server()
{
    mTcpServer.close();
    if(mSocket)
        mSocket->close();
    mSocket->waitForDisconnected();
    if(mServer)
        delete mServer;
    if(mSocket)
        delete mSocket;
}

void Server::restart(QString host, int port)
{
    QSettings settings;
    QHostAddress address;
    if(host.compare("all", Qt::CaseInsensitive) || host.compare("any", Qt::CaseInsensitive))
        address = QHostAddress::Any;
    else
        address = host;

    if(mTcpServer.isListening())
    {
        mTcpServer.close();
        emit stateChanged(QAbstractSocket::UnconnectedState);
    }
    if(mTcpServer.listen(address, port))
    {
        emit stateChanged(QAbstractSocket::ListeningState);
        settings.setValue("port", port);
        settings.setValue("host", host);
    }
    else
    {
        emit listenError(mTcpServer.errorString());
    }
}

void Server::onConnect()
{
   mSocket = mTcpServer.nextPendingConnection();
    if(!mSocket)
        return;
    connect(mSocket, SIGNAL(disconnected()), SLOT(onDisconnect()));
    emit stateChanged(QAbstractSocket::ConnectedState);
    mServer = new RemoteRcUnitServer(&mRcUnits, mSocket, true);
}

void Server::onDisconnect()
{
    emit stateChanged(QAbstractSocket::UnconnectedState);
    if(mServer)
        delete mServer;
    if(mSocket)
        mSocket->deleteLater();
    mServer = 0;
    mSocket = 0;

}

void Server::loadProject(const QString &filename)
{
    mRcUnits.loadConfig(filename);
}
