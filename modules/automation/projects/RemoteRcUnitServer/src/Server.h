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

#ifndef SERVER_H
#define SERVER_H

#include <QThread>
#include <QTcpServer>
#include <QMutex>

#include "remoteSignals/RemoteRcUnitServer.h"

#include <core/RcUnitHelp.h>
#include "RcUnits.h"

class Server : public QObject
{
    Q_OBJECT
public:
    Server();
    virtual ~Server();
signals:
    void stateChanged(int state);
    void listenError(const QString& error);
    void rcUnitListRequested();

public slots:
    void restart(QString host, int port);
    void loadProject(const QString& filename);

private slots:
    void onConnect();
    void onDisconnect();

private:
    QTcpServer mTcpServer;
    QTcpSocket* mSocket;
    RcUnits mRcUnits;
    RemoteRcUnitServer* mServer;
};

#endif // SERVER_H
