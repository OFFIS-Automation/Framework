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

#include "RemoteRcUnit.h"
#include "RemoteRcUnits.h"
#include "TcInvoker.h"
#include "remoteSignals/RemoteRcUnitClient.h"

RemoteRcUnit::RemoteRcUnit(const RcUnitHelp &help, RemoteRcUnitClient &remoteRcUnits) :
    mRemote(remoteRcUnits)
{
    mHelp = help;
}

RemoteRcUnit::~RemoteRcUnit()
{

}

QVariant RemoteRcUnit::call(const QByteArray &method, QList<QVariant> params)
{
    return mRemote.callMethod(mHelp.unitName.toLatin1(), method, params);
}

void RemoteRcUnit::connectGamepad(QObject *gamepad)
{
    mRemote.enableTelecontrol(name());
    connect(gamepad, SIGNAL(buttonToggled(int,bool)), SLOT(setGamepadButton(int,bool)), Qt::DirectConnection);
    connect(gamepad, SIGNAL(dataUpdated(QMap<int,double>)), SLOT(handleGamepadData(QMap<int,double>)), Qt::DirectConnection);

}

void RemoteRcUnit::disconnectGamepad(QObject *gamepad)
{
    gamepad->disconnect(this);
    mRemote.disableTelecontrol(name());
}

void RemoteRcUnit::updateGamepadParameters(const QString &unitName, double sensitivity, const QList<bool> &inverts)
{
    mRemote.updateGamepadParameters(name(), unitName, sensitivity, inverts);
}

void RemoteRcUnit::acquire()
{
 //@TODO
}

void RemoteRcUnit::release()
{
    //@TODO
}

void RemoteRcUnit::stop()
{
    //@TODO
}

bool RemoteRcUnit::acquired()
{
    //@TODO
    return false;
}

void RemoteRcUnit::setObserver(RcUnitBaseObserver *observer)
{
    //@TODO
    Q_UNUSED(observer)
}

QVariantList RemoteRcUnit::getFlags()
{
    //TODO
    return QVariantList();
}

void RemoteRcUnit::updateTelecontrolAssignment(const QString &telecontrolDeviceName)
{
   //@TODO
    Q_UNUSED(telecontrolDeviceName)
}

void RemoteRcUnit::handleGamepadData(const QMap<int, double> &data)
{
    mRemote.handleTcData(data, 1000);
}

void RemoteRcUnit::setGamepadButton(int id, bool pressed)
{
    mRemote.setTcButton(id, pressed, 2000);
}
