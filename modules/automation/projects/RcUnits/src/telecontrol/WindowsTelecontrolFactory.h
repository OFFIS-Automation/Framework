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

#ifndef WINDOWSTELECONTROLFACTORY_H
#define WINDOWSTELECONTROLFACTORY_H

#include "Gamepad.h"
#include <dinput.h>

class WindowsTelecontrolFactory : public QObject
{
public:
    static WindowsTelecontrolFactory& instance();
    static QMap<QString, Gamepad *> getGamepadDevices();
    ~WindowsTelecontrolFactory();

private:
    WindowsTelecontrolFactory();

    static BOOL CALLBACK enumDevices(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);
    static void enumConnexionDevices();

    static LPDIRECTINPUT8 sDirectInput;
    static QMap<QString, Gamepad *> sGamepadDevices;
    static QStringList disallowedControllerNames;
};

#endif // WINDOWSTELECONTROLFACTORY_H
