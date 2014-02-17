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


#ifndef REMOTERCUNITS_H
#define REMOTERCUNITS_H


#include <QObject>
#include <QTcpSocket>
#include <QMutex>
#include <QWaitCondition>

#include <core/RcUnitHelp.h>

#include "RemoteClientGlobal.h"

class RcUnitBase;
class RemoteRcUnitClient;
class REMOTECLIENT_EXPORT RemoteRcUnits : public QObject
{
    Q_OBJECT
public:
    explicit RemoteRcUnits(const QString& name, const QString& host, int port);
    virtual ~RemoteRcUnits();
    QList<RcUnitBase*> units();
    double timout();
    QString name() const { return mName; }

signals:
    void unitsUpdated(const QString& remoteServerName, const QStringList& oldLolecs);

public slots:
    void startConnect();
    void setTimeout(double seconds);

private slots:
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);

private:
    QString mName;
    QString mHost;
    int mPort;
    QTcpSocket mSocket;
    QMap<QString, RcUnitHelp> mLolecs;
    RemoteRcUnitClient* mClient;
};

#endif // REMOTERCUNITS_H
