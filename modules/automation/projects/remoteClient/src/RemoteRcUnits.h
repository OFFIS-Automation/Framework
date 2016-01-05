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


#ifndef REMOTERCUNITS_H
#define REMOTERCUNITS_H


#include <QThread>
#include <QTcpSocket>
#include <QMutex>
#include <QWaitCondition>

#include <core/RcUnitHelp.h>

#include "RemoteClientGlobal.h"

class RcUnitBase;
class RemoteRcUnitClient;

class REMOTECLIENT_EXPORT RemoteRcUnits : public QThread
{
    Q_OBJECT
public:
    explicit RemoteRcUnits(const QString& name, const QString& host, int port, double timeout = 10.0);
    virtual ~RemoteRcUnits();
    QList<RcUnitBase*> units();
    double timout();
    QString name() const { return mName; }
    void startConnect();

signals:
    void unitsUpdated(const QString& remoteServerName, const QStringList& oldRcUnits);
public slots:
    void setTimeout(double seconds);

private slots:
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);

private:
    void run();
    QString mName;
    QString mHost;
    int mPort;
    double mTimeout;
    QTcpSocket* mSocket;
    QMap<QString, RcUnitHelp> mRcUnits;
    RemoteRcUnitClient* mClient;
    QMutex mMutex;
    QWaitCondition mStartupWait;
};

#endif // REMOTERCUNITS_H
