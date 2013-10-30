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

#ifndef LOLECTOOLS_TCCONFIG_H
#define LOLECTOOLS_TCCONFIG_H

#include <QList>


namespace Tc
{

    enum Button
    {
        NorthButton,
        SouthButton,
        WestButton,
        EastButton,
        LeftShoulderUpperButton,
        LeftShoulderLowerButton,
        RightShoulderUpperButton,
        RightShoulderLowerButton
    };

    enum Joystick
    {
        NoJoystick = -1,
        LeftJoystickX = 0,
        LeftJoystickY,
        RightJoystickX,
        RightJoystickY
    };
    inline QList<Joystick> joysticks()
    {
        return QList<Joystick>();
    }

    inline QList<Joystick> joysticks(Joystick j1)
    {
        return joysticks() << j1;
    }

    inline QList<Joystick> joysticks(Joystick j1, Joystick j2)
    {
        return joysticks() << j1 << j2;
    }

    inline QList<Joystick> joysticks(Joystick j1, Joystick j2, Joystick j3)
    {
        return joysticks() << j1 << j2 << j3;
    }
    inline QList<Joystick> joysticks(Joystick j1, Joystick j2, Joystick j3, Joystick j4)
    {
        return joysticks() << j1 << j2 << j3 << j4;
    }
}
#endif //LOLECTOOLS_TCCONFIG_H
