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
        FootboardNorthButton,
        FootboardSouthButton,
        FootboardEastButton,
        FootboardWestButton,
        ButtonEnumEnd,
        ButtonEnumFirst = NorthButton
    };

    enum Joystick
    {
        NoJoystick = -1,
        LeftJoystickX = 0,
        LeftJoystickY,
        RightJoystickX,
        RightJoystickY,
        JoystickX,
        JoystickY,
        JoystickZ,
        JoystickYaw,
        JoystickPitch,
        JoystickRoll
    };

    enum HapticAxis
    {
        NoAxis = -1,
        HapticAxisX = 0,
        HapticAxisY,
        HapticAxisZ,
        HapticAxisYaw,
        HapticAxisPitch,
        HapticAxisRoll
    };

    enum HapticButton
    {
        UnknownHapticButton = -1,
        PrimaryButton = 0,
        SecondaryButton
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
                return "North button";
            case SouthButton:
                return "South button";
            case WestButton:
                return "West button";
            case EastButton:
                return "East button";
            case LeftShoulderUpperButton:
                return "Left shoulder upper button";
            case LeftShoulderLowerButton:
                return "Left shoulder lower button";
            case RightShoulderUpperButton:
                return "Right shoulder upper button";
            case RightShoulderLowerButton:
                return "Right shoulder lower button";
            default:
                return "Unknown button";
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

    inline QList<HapticAxis> hapticAxis()
    {
        return QList<HapticAxis>();
    }

    inline QList<HapticAxis> hapticAxis(HapticAxis a1)
    {
        return hapticAxis() << a1;
    }

    inline QList<HapticAxis> hapticAxis(HapticAxis a1, HapticAxis a2)
    {
        return hapticAxis() << a1 << a2;
    }

    inline QList<HapticAxis> hapticAxis(HapticAxis a1, HapticAxis a2, HapticAxis a3)
    {
        return hapticAxis() << a1 << a2 << a3;
    }

    inline QList<HapticAxis> hapticAxis(HapticAxis a1, HapticAxis a2, HapticAxis a3, HapticAxis a4)
    {
        return hapticAxis() << a1 << a2 << a3 << a4;
    }

    inline QList<HapticAxis> hapticAxis(HapticAxis a1, HapticAxis a2, HapticAxis a3, HapticAxis a4, HapticAxis a5)
    {
        return hapticAxis() << a1 << a2 << a3 << a4 << a5;
    }

    inline QList<HapticAxis> hapticAxis(HapticAxis a1, HapticAxis a2, HapticAxis a3, HapticAxis a4, HapticAxis a5, HapticAxis a6)
    {
        return hapticAxis() << a1 << a2 << a3 << a4 << a5 << a6;
    }

    inline QList<HapticAxis> allHapticAxis() {
        return hapticAxis(Tc::HapticAxisX, Tc::HapticAxisY, Tc::HapticAxisZ, Tc::HapticAxisYaw, Tc::HapticAxisPitch, Tc::HapticAxisRoll);
    }

    inline QString stringForHapticButton(int button)
    {
        switch(button){
            case PrimaryButton:
                return "PrimaryButton";
            case SecondaryButton:
                return "SecondaryButton";
            default:
                return "UnknownButton";
        }
    }

    inline QString userFriendlyStringForHapticButton(int button)
    {
        switch(button){
            case PrimaryButton:
                return "Primary button";
            case SecondaryButton:
                return "Secondary button";
            default:
                return "Unknown haptic button";
        }
    }

    inline HapticButton hapticButtonFromString(QString buttonName)
    {
        if(buttonName == "PrimaryButton")
            return PrimaryButton;
        else if(buttonName == "SecondaryButton")
            return SecondaryButton;
        else
            return UnknownHapticButton;
    }

    inline QString stringForHapticAxis(int axis)
    {
        switch(axis){
            case HapticAxisX:
                return "HapticAxisX";
            case HapticAxisY:
                return "HapticAxisY";
            case HapticAxisZ:
                return "HapticAxisZ";
            case HapticAxisYaw:
                return "HapticAxisYaw";
            case HapticAxisPitch:
                return "HapticAxisPitch";
            case HapticAxisRoll:
                return "HapticAxisRoll";
            default:
                return "UnknownAxis";
        }
    }

    inline HapticAxis hapticAxisFromString(const QString& name)
    {
        if(name == "HapticAxisX")
            return HapticAxisX;
        else if(name == "HapticAxisY")
            return HapticAxisY;
        else if(name == "HapticAxisZ")
            return HapticAxisZ;
        else if(name == "HapticAxisYaw")
            return HapticAxisYaw;
        else if(name == "HapticAxisPitch")
            return HapticAxisPitch;
        else if(name == "HapticAxisRoll")
            return HapticAxisRoll;
        else
            return NoAxis;
    }

}

#endif //LOLECTOOLS_TCCONFIG_H
