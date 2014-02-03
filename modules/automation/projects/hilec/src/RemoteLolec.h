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

#ifndef REMOTELOLEC_H
#define REMOTELOLEC_H

#include <QObject>
#include <QTcpSocket>
#include <QMutex>
#include <QWaitCondition>

#include <core/RcUnitHelp.h>

class RcUnitBase;

class RemoteLolec : public QObject
{
    Q_OBJECT
public:
    explicit RemoteLolec(const QString& name, const QString& host, int port);
    virtual ~RemoteLolec();
    QList<RcUnitBase*> units();
    double timout();
    QString name() const { return mName; }

signals:
    void lolecsUpdated(const QString& remoteServerName, const QStringList& oldLolecs);
    void sendMessage(const QByteArray& msgData);

public slots:
    void startConnect();
    QVariant callMethod(const QByteArray &unit, const QByteArray &name, const QList<QVariant> &params);
    void updateSensitivity(const QString& unit, const QString &tcName, double sensitivity, const QList<bool> &inverts);
    void connectGamepad(const QString& unitName);
    void disconnectGamepad(const QString& unitName);
    void handleGamepadData(const QMap<int,double>& data);
    void setGamepadButton(int id, bool pressed);
    void setTimeout(double seconds);

private slots:
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);
    void onReadyRead();
    void onSendMessage(const QByteArray& msgData);
private:
    void updateGamepad(const QString& cmd, const QString& unit);
    void getLolecs();
    void dispatch(const QString& method, QDataStream& stream);
    void decodeListResponse(const QVariant& data);
    QString mName;
    QString mHost;
    int mPort;
    QTcpSocket mSocket;
    int mReadSize, mReadId;
    QMap<QString, RcUnitHelp> mLolecs;

    QMutex mMutex, mDataMutex;

    struct RemoteCall
    {
        QWaitCondition waiter;
        QVariant response;
        QString error;
        bool hasError;
    };
    QWaitCondition tcWaiter;
    QMap<int, RemoteCall*> mCalls;
    int mNextCallId;
    ulong mTimeout;
};

#endif // REMOTELOLEC_H
