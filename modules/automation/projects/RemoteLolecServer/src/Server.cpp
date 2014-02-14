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

#include "Server.h"
#include <QSettings>
#include <QTcpSocket>
#include <QtEndian>
#include <QStringList>
#include <QDebug>

Server::Server() : mTcpServer(this)
{
    connect(&mTcpServer, SIGNAL(newConnection()), SLOT(onConnect()));
/*
    connect(&mRcUnits, SIGNAL(asyncResponse(int,QVariant)), SLOT(sendCallResponse(int,QVariant)));
    connect(&mRcUnits, SIGNAL(asyncError(int,QString)), SLOT(sendCallError(int,QString)));
    */
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
    emit stateChanged(QAbstractSocket::ConnectedState);
    mServer = new RemoteRcUnitServer(&mRcUnits, mSocket, true);
}

void Server::onDisconnect()
{
}
/*
void Server::dispatch(const QString &method, QDataStream &stream)
{
    qDebug() << method;
    if(method == "list")
    {
        sendLolecList();
    }
    else if(method == "call")
    {
        QByteArray method, unit;
        int id;
        QList<QVariant> params;
        stream >> unit >> method >> id >> params;
        mRcUnits.callAsync(unit, method, id, params);
    }
    else if(method.left(2) == "tc")
    {
        int id;
        stream >> id;

        if(method == "tcSens")
        {
            double sens;
            QList<bool> inverts;
            QString unit, tcName;
            stream >> unit >> tcName >> sens >> inverts;
            qDebug() << "tcSens" << id << unit;
            mRcUnits.updateSensitivity(unit, tcName, sens, inverts);
        }
        else if(method == "tcEnable")
        {
            QString unit;
            stream >> unit;
            qDebug() << "tcEnable" << id << unit;
            mRcUnits.enableGamepad(unit);
        }
        else if(method == "tcDisable")
        {
            QString unit;
            stream >> unit;
            qDebug() << "tcDisable" << id << unit;
            mRcUnits.disableGamepad(unit);
        }
        else if(method == "tcData")
        {
            QMap<int, double> data;
            stream >> data;
            mRcUnits.handleTcData(data);
        }
        else if(method == "tcButton")
        {
            int button;
            bool enable;
            stream >> button >> enable;
            qDebug() << "tcButton" << id << button << enable;
            mRcUnits.handleTcButton(button, enable);
        }
        sendTcResponse(id);
    }
}

void Server::sendLolecList()
{
    const QMap<QString, RcUnitHelp>& list = mRcUnits.getHelpList();
    QList<QVariant> data;
    foreach(const QString& name, list.keys())
    {
        const RcUnitHelp& help = list.value(name);
        QList<QVariant> structs;
        foreach(const RcUnitHelp::Struct& structItem, help.structs)
        {
            QStringList structData;
            structData << structItem.name;
            structData << structItem.members;
            structs << QVariant(structData);
        }
        QList<QVariant> methods;
        foreach(const RcUnitHelp::Method& method, help.methods)
        {
            QStringList methodData;
            methodData << method.name << method.sig << method.shortDesc << method.longDesc;
            methods << QVariant(methodData);
        }
        QList<QVariant> tcMethods;
        foreach(const RcUnitHelp::TcJostick& method, help.tcJoysticks)
        {
            QVariantList methodData;
            methodData << method.name << method.deadMansButton << method.sensitivity << QVariant(method.axeNames);
            tcMethods << QVariant(methodData);
        }

        data << name;
        data << help.desc;
        data << QVariant(structs);
        data << QVariant(methods);
        data << QVariant(help.constants);
        data << QVariant(tcMethods);
    }
    QByteArray byteData;
    QDataStream stream(&byteData, QIODevice::WriteOnly);
    stream << QString("list");
    stream << QVariant(data);
    sendMessage(byteData);
}
*/

void Server::loadProject(const QString &filename)
{
    mRcUnits.loadConfig(filename);
}
