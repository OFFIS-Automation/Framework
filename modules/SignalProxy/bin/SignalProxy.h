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

#ifndef SIGNALPROXY_H
#define SIGNALPROXY_H

#include <QObject>
#include <QIODevice>
#include <QDataStream>
#include <QUuid>
#include <QVariant>

class SignalProxy : public QObject
{
    Q_OBJECT
public:
    explicit SignalProxy(quint64 gid, QIODevice& readDevice, QIODevice& writeDevice);

protected slots:
    void transmitSignal(const QByteArray& msgData);

protected:
    quint64 globalId() { return mGlobalId; }
    void handleError(int id);
    void checkId(quint64 globalId);
    virtual void processRemoteInputs(const QByteArray& data) = 0;
private slots:
    void onReadyRead();

private:
    int mReadSize;
    const quint64 mGlobalId;
    QIODevice& mReadDevice;
    QIODevice& mWriteDevice;
};

#endif // SIGNALPROXY_H
