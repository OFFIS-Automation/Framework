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

#ifndef REMOTERCUNIT_H
#define REMOTERCUNIT_H

#include "RcUnitBase.h"
class RemoteLolec;
class TcInvoker;

class RemoteRcUnit : public QObject, public RcUnitBase
{
    Q_OBJECT
public:
    RemoteRcUnit(const QString& name, const RcUnitHelp& help, RemoteLolec& remoteLolec);
    virtual ~RemoteRcUnit();
    virtual QVariant call(const QByteArray& method, QList<QVariant> params);
    virtual RcUnitHelp getHelp() { return mHelp; }
    virtual QObject* lolec() { return 0; }
    virtual QString name() { return mName; }
    virtual void connectGamepad(QObject* gamepad );
    virtual void disconnectGamepad(QObject* );
    virtual void updateSensitivity(const QString& unitName, double sensitivity, const QList<bool>& inverts);
    QMap<QString, QVariant> getConstants() { return mHelp.constants; }
    bool isTelecontrolable() const { return !mHelp.tcJoysticks.empty(); }
    bool hasHapticInterface() const { return false; }
    const HapticResponse currentHapticData() { return HapticResponse(); }
    const HapticResponse hapticMovement(const QVector3D&) { return HapticResponse(); }
    virtual void updateHapticSensitivity(double, double){}
    virtual double hapticSensitivity() const { return 0.0; }
    virtual double hapticForceFactor() const { return 0.0; }
public slots:
    void handleGamepadData(const QMap<int,double>& data);
    void setGamepadButton(int id, bool pressed);
protected:
    QString mName;
    RcUnitHelp mHelp;
    RemoteLolec& mRemote;
};

#endif // REMOTERCUNIT_H
