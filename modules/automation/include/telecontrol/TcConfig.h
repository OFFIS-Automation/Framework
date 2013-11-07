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
        UnknownButton = -1,
        NorthButton = 0,
        SouthButton,
        WestButton,
        EastButton,
        LeftShoulderUpperButton,
        LeftShoulderLowerButton,
        RightShoulderUpperButton,
        RightShoulderLowerButton,
        ButtonEnumEnd,
        ButtonEnumFirst = NorthButton
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

    inline QList<Joystick> allJoysticks() {
        return joysticks(Tc::LeftJoystickX, Tc::LeftJoystickY, Tc::RightJoystickX, Tc::RightJoystickY);
    }

    inline QString stringForButton(int button)
    {
        switch(button){
            case NorthButton:
                return "NorthButton";
            case SouthButton:
                return "SouthButton";
            case WestButton:
                return "WestButton";
            case EastButton:
                return "EastButton";
            case LeftShoulderUpperButton:
                return "LeftShoulderUpperButton";
            case LeftShoulderLowerButton:
                return "LeftShoulderLowerButton";
            case RightShoulderUpperButton:
                return "RightShoulderUpperButton";
            case RightShoulderLowerButton:
                return "RightShoulderLowerButton";
            default:
                return "UnknownButton";
        }
    }

    inline QString userFriendlyStringForButton(int button)
    {
        switch(button){
            case NorthButton:
                return "north button";
            case SouthButton:
                return "south button";
            case WestButton:
                return "west button";
            case EastButton:
                return "east button";
            case LeftShoulderUpperButton:
                return "left shoulder upper button";
            case LeftShoulderLowerButton:
                return "left shoulder lower button";
            case RightShoulderUpperButton:
                return "right shoulder upper button";
            case RightShoulderLowerButton:
                return "right shoulder lower button";
            default:
                return "unknown button";
        }
    }

    inline Button buttonFromString(QString buttonName)
    {
        if(buttonName == "NorthButton")
            return NorthButton;
        else if(buttonName == "SouthButton")
            return SouthButton;
        else if(buttonName == "WestButton")
            return WestButton;
        else if(buttonName == "EastButton")
            return EastButton;
        else if(buttonName == "LeftShoulderUpperButton")
            return LeftShoulderUpperButton;
        else if(buttonName == "LeftShoulderLowerButton")
            return LeftShoulderLowerButton;
        else if(buttonName == "RightShoulderUpperButton")
            return RightShoulderUpperButton;
        else if(buttonName == "RightShoulderLowerButton")
            return RightShoulderLowerButton;
        else
            return UnknownButton;
    }

    inline QString stringForJoystick(int joystick)
    {
        switch(joystick){
            case LeftJoystickX:
                return "LeftJoystickX";
            case LeftJoystickY:
                return "LeftJoystickY";
            case RightJoystickX:
                return "RightJoystickX";
            case RightJoystickY:
                return "RightJoystickY";
            default:
                return "UnknownJoystick";
        }
    }
    inline Joystick joystickFromString(const QString& name)
    {
        if(name == "LeftJoystickX")
            return LeftJoystickX;
        else if(name == "LeftJoystickY")
            return LeftJoystickY;
        else if(name == "RightJoystickX")
            return RightJoystickX;
        else if(name == "RightJoystickY")
            return RightJoystickY;
        else
            return NoJoystick;
    }
}
#endif //LOLECTOOLS_TCCONFIG_H
