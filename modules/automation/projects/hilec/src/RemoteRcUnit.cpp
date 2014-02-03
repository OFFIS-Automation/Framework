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

#include "RemoteRcUnit.h"
#include "RemoteLolec.h"
#include "TcInvoker.h"

RemoteRcUnit::RemoteRcUnit(const QString &name, const RcUnitHelp &help, RemoteLolec &remoteLolec) :
    mRemote(remoteLolec)
{
    mName = name;
    mHelp = help;
    mHelp.server = mRemote.name();
}

RemoteRcUnit::~RemoteRcUnit()
{

}

QVariant RemoteRcUnit::call(const QByteArray &method, QList<QVariant> params)
{
    return mRemote.callMethod(mName.toLatin1(), method, params);
}

void RemoteRcUnit::connectGamepad(QObject *gamepad)
{
    mRemote.connectGamepad(mName);
    connect(gamepad, SIGNAL(buttonToggled(int,bool)), SLOT(setGamepadButton(int,bool)), Qt::DirectConnection);
    connect(gamepad, SIGNAL(dataUpdate(QMap<int,double>)), SLOT(handleGamepadData(QMap<int,double>)), Qt::DirectConnection);

}

void RemoteRcUnit::disconnectGamepad(QObject *gamepad)
{
    gamepad->disconnect(this);
    mRemote.disconnectGamepad(mName);
}

void RemoteRcUnit::updateSensitivity(const QString &unitName, double sensitivity, const QList<bool> &inverts)
{
    mRemote.updateSensitivity(mName, unitName, sensitivity, inverts);
}

void RemoteRcUnit::handleGamepadData(const QMap<int, double> &data)
{
    mRemote.handleGamepadData(data);
}

void RemoteRcUnit::setGamepadButton(int id, bool pressed)
{
    mRemote.setGamepadButton(id, pressed);
}
