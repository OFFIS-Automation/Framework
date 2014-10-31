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

#ifndef TELECONTROLFACTORY_H
#define TELECONTROLFACTORY_H

// QT headers
#include <QObject>
#include <QMap>

// Project related headers
#include "Gamepad.h"
#include "telecontrol/HapticInterface.h"
#include "../RcUnitsGlobal.h"

class RCUNITS_EXPORT TelecontrolFactory : public QObject
{
    Q_OBJECT
public:
    static QMap<QString, Gamepad *> getGamepadDevices();
    static QMap<QString, HapticDevice *> getHapticDevices();

private:
    Q_DISABLE_COPY(TelecontrolFactory)
    explicit TelecontrolFactory(QObject *parent = 0);
    static TelecontrolFactory& instance();
    QMap<QString, HapticDevice *> loadHapticDevices();
};

#endif // TELECONTROLFACTORY_H
