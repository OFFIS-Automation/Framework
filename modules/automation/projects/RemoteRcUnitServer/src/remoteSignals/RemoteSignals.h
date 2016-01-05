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
// along with this program.  If not, see <http:#www.gnu.org/licenses/>.

#ifndef REMOTESIGNALS_H
#define REMOTESIGNALS_H

#include <QObject>
#include <QIODevice>
#include <QDataStream>
#include <QStringList>
#include <QVariant>
#include <QMutex>

class RemoteSignals : public QObject
{
    Q_OBJECT
public:
    explicit RemoteSignals(quint64 gid1, quint64 gid2, QIODevice* readDevice, QIODevice* writeDevice, bool initialize = false);
    void initialize();

signals:
	void transmitSignal(const QByteArray& msgData);
private slots:
    void transmitSignalAsync(const QByteArray& msgData);

protected:
	const int version() const { return 1; }
    const quint64 gid1() const { return mGlobalId1; }
    const quint64 gid2() const { return mGlobalId2; }
    void handleError(int id);
    void checkId(int version, quint64 globalId1, quint64 globalId2);
    virtual void processRemoteInputs(const QByteArray& data) = 0;
private slots:
    void onReadyRead();

private:
    int mReadSize;
    const quint64 mGlobalId1, mGlobalId2;
    QIODevice* mReadDevice;
    QIODevice* mWriteDevice;
    QMutex mMutex;
};

#endif // REMOTESIGNALS_H
