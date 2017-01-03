// OFFIS Automation Framework
// Copyright (C) 2013-2017 OFFIS e.V.
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

#ifndef REMOTERCUNIT_H
#define REMOTERCUNIT_H

#include "RcUnitBase.h"

class RemoteRcUnitClient;
class TcInvoker;

class RemoteRcUnit : public QObject, public RcUnitBase
{
    Q_OBJECT
public:
    RemoteRcUnit(const RcUnitHelp& help, RemoteRcUnitClient& remoteRcClient);
    virtual ~RemoteRcUnit();
    virtual QVariant call(const QByteArray& method, QList<QVariant> params);
    virtual RcUnitHelp getHelp() const { return mHelp; }
    virtual UserRcUnit* rcUnit() { return 0; }
    virtual QString name() const { return mHelp.unitName; }

    void acquire();
    void release();
    void stop();
    bool acquired();
    void setObserver(RcUnitBaseObserver *observer);
    QVariantList getFlags();

    virtual void updateTelecontrolAssignment(const QString& telecontrolDeviceName);

    virtual void connectGamepad(QObject* gamepad );
    virtual void disconnectGamepad(QObject* );
    virtual void updateGamepadParameters(const QString& unitName, double sensitivity, const QList<bool>& inverts);

    QMap<QString, QVariant> getConstants() const { return mHelp.constants; }

    bool hasGamepadControl() const { return !mHelp.tcGamepadMoves.empty(); }

    void connectHapticDevice(QObject*) {}
    void disconnectHapticDevice(QObject*) {}
    void updateHapticParameters(const QString&, double, double, const QList<bool>&) {}
    bool hasHapticControl() const { return false; }

public slots:
    void handleGamepadData(const QMap<int,double>& data);
    void setGamepadButton(int id, bool pressed);
protected:
    RcUnitHelp mHelp;
    RemoteRcUnitClient& mRemote;
};

#endif // REMOTERCUNIT_H
