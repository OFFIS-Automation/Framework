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

#ifndef TELECONTROLFACTORY_H
#define TELECONTROLFACTORY_H

#include <QObject>
#include "Gamepad.h"
#include <telecontrol/HapticInterface.h>

class TelecontrolFactory : public QObject
{
    Q_OBJECT
public:
    static Gamepad* createGamepad();
    static HapticInterface* createHaptic();
private:
    Q_DISABLE_COPY(TelecontrolFactory)
    explicit TelecontrolFactory(QObject *parent = 0);
    static TelecontrolFactory& instance();

    HapticInterface* mHaptic;
};

#endif // TELECONTROLFACTORY_H
