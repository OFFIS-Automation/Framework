// OFFIS Automation Framework
// Copyright (C) 2013-2016 OFFIS e.V.
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

#ifndef RCUNITTOOLS_TCCONFIG_H
#define RCUNITTOOLS_TCCONFIG_H

#include <QList>

namespace Tc
{
    namespace Gamepad
    {
        enum WheelButtons {
            ButtonUp = 256,
            ButtonDown,
            ButtonLeft,
            ButtonRight
        };
        enum Buttons
        {
            NorthButton = 0,
            SouthButton,
            WestButton,
            EastButton,
            LeftShoulderUpperButton,
            LeftShoulderLowerButton,
            RightShoulderUpperButton,
            RightShoulderLowerButton,
            TriggerButton, // toggles when one of the trigger buttons on the XBOX gamepad is pressed
            ImplicitActivationButton, // toggles when any joystick is above zero Use this if you don't want a dead mans control
            FirstButton = NorthButton,
            LastButton = ImplicitActivationButton
        };
        enum Joystick
        {
            LeftJoystickX = 0,
            LeftJoystickY,
            RightJoystickX,
            RightJoystickY,
            TriggerJoystick
        };

    }
    namespace Connexion
    {
        enum Buttons
        {
            MenuButton = 512,
            FitButton,
            ImplicitActivationButton
        };
        enum QuickViewButtons
        {
            TopBottomButton = 1024,
            RightLeftButton,
            FrontBackButton,
            CWCCWButton,
            RotateButton,
            ISO1ISO2Button,
            View1Button, //
            View2Button, //
            View3Button //
        };
        enum ModifierButtons
        {
            CtrlButton = 2048,
            AltButton,
            ShiftButton,
            EscButton,
            EnterButton,
            DeleteButton,
            SpaceButton, //
            TabButton
        };
        enum FunctionButtons
        {
            OneButton = 5096,
            TwoButton,
            ThreeButton,
            FourButton,
            FiveButton,
            SixButton,
            SevenButton,
            EightButton,
            NineButton,
            TenButton,
            ElevenButton,
            TwelveButton
        };

        enum Joystick
        {
            JoystickX = 64,
            JoystickY,
            JoystickZ,
            JoystickYaw,
            JoystickPitch,
            JoystickRoll,
        };
    }
    namespace Virtual
    {
        enum VirtualButtons
        {
            CtrlF1Button = 128,
            CtrlF2Button,
            CtrlF3Button,
            // Footboard buttons
            FootboardButton1 = CtrlF1Button,
            FootboardButton2 = CtrlF2Button,
            FootboardButton3 = CtrlF3Button,
            FirstButton = CtrlF1Button,
            LastButton = CtrlF3Button
        };
    }

    namespace Haptic
    {
        enum Axis
        {
            NoAxis = -1,
            AxisX = 192,
            AxisY,
            AxisZ,
            AxisYaw,
            AxisPitch,
            AxisRoll
        };

        enum Button
        {
            UnknownHapticButton = -1,
            PrimaryButton = 192,
            SecondaryButton,
            FirstButton = PrimaryButton,
            LastButton = SecondaryButton
        };
    }


    inline QList<Gamepad::Joystick> joysticks(Gamepad::Joystick j1)
    {
        QList<Gamepad::Joystick> l;
        return l << j1;
    }

    inline QList<Gamepad::Joystick> joysticks(Gamepad::Joystick j1, Gamepad::Joystick j2)
    {
        return joysticks(j1) << j2;
    }

    inline QList<Gamepad::Joystick> joysticks(Gamepad::Joystick j1, Gamepad::Joystick j2, Gamepad::Joystick j3)
    {
        return joysticks(j1, j2) << j3;
    }
    inline QList<Gamepad::Joystick> joysticks(Gamepad::Joystick j1, Gamepad::Joystick j2, Gamepad::Joystick j3, Gamepad::Joystick j4)
    {
        return joysticks(j1, j2, j3) << j4;
    }

    inline QList<Gamepad::Joystick> joysticks(Gamepad::Joystick j1, Gamepad::Joystick j2, Gamepad::Joystick j3, Gamepad::Joystick j4, Gamepad::Joystick j5)
    {
        return joysticks(j1, j2, j3, j4) << j5;
    }

    inline QList<Gamepad::Joystick> joysticks(Gamepad::Joystick j1, Gamepad::Joystick j2, Gamepad::Joystick j3, Gamepad::Joystick j4, Gamepad::Joystick j5, Gamepad::Joystick j6)
    {
        return joysticks(j1, j2, j3, j4, j5) << j6;
    }

    inline QList<Gamepad::Joystick> allGamepadJoysticks() {
        return joysticks(Tc::Gamepad::LeftJoystickX, Tc::Gamepad::LeftJoystickY, Tc::Gamepad::RightJoystickX, Tc::Gamepad::RightJoystickY);
    }

    inline QList<Connexion::Joystick> joysticks(Connexion::Joystick j1)
    {
        QList<Connexion::Joystick> l;
        return l << j1;
    }

    inline QList<Connexion::Joystick> joysticks(Connexion::Joystick j1, Connexion::Joystick j2)
    {
        return joysticks(j1) << j2;
    }

    inline QList<Connexion::Joystick> joysticks(Connexion::Joystick j1, Connexion::Joystick j2, Connexion::Joystick j3)
    {
        return joysticks(j1, j2) << j3;
    }
    inline QList<Connexion::Joystick> joysticks(Connexion::Joystick j1, Connexion::Joystick j2, Connexion::Joystick j3, Connexion::Joystick j4)
    {
        return joysticks(j1, j2, j3) << j4;
    }

    inline QList<Connexion::Joystick> joysticks(Connexion::Joystick j1, Connexion::Joystick j2, Connexion::Joystick j3, Connexion::Joystick j4, Connexion::Joystick j5)
    {
        return joysticks(j1, j2, j3, j4) << j5;
    }

    inline QList<Connexion::Joystick> joysticks(Connexion::Joystick j1, Connexion::Joystick j2, Connexion::Joystick j3, Connexion::Joystick j4, Connexion::Joystick j5, Connexion::Joystick j6)
    {
        return joysticks(j1, j2, j3, j4, j5) << j6;
    }

    inline QList<Connexion::Joystick> allConnexionJoysticks() {
        return joysticks(Tc::Connexion::JoystickX, Tc::Connexion::JoystickY, Tc::Connexion::JoystickZ, Tc::Connexion::JoystickYaw, Tc::Connexion::JoystickPitch, Tc::Connexion::JoystickRoll);
    }

    inline QList<Haptic::Axis> hapticAxis()
    {
        return QList<Haptic::Axis>();
    }

    inline QList<Haptic::Axis> hapticAxis(Haptic::Axis a1)
    {
        return hapticAxis() << a1;
    }

    inline QList<Haptic::Axis> hapticAxis(Haptic::Axis a1, Haptic::Axis a2)
    {
        return hapticAxis() << a1 << a2;
    }

    inline QList<Haptic::Axis> hapticAxis(Haptic::Axis a1, Haptic::Axis a2, Haptic::Axis a3)
    {
        return hapticAxis() << a1 << a2 << a3;
    }

    inline QList<Haptic::Axis> hapticAxis(Haptic::Axis a1, Haptic::Axis a2, Haptic::Axis a3, Haptic::Axis a4)
    {
        return hapticAxis() << a1 << a2 << a3 << a4;
    }

    inline QList<Haptic::Axis> hapticAxis(Haptic::Axis a1, Haptic::Axis a2, Haptic::Axis a3, Haptic::Axis a4, Haptic::Axis a5)
    {
        return hapticAxis() << a1 << a2 << a3 << a4 << a5;
    }

    inline QList<Haptic::Axis> hapticAxis(Haptic::Axis a1, Haptic::Axis a2, Haptic::Axis a3, Haptic::Axis a4, Haptic::Axis a5, Haptic::Axis a6)
    {
        return hapticAxis() << a1 << a2 << a3 << a4 << a5 << a6;
    }

    inline QList<Haptic::Axis> allHapticAxis() {
        return hapticAxis(Haptic::AxisX, Tc::Haptic::AxisY, Tc::Haptic::AxisZ, Tc::Haptic::AxisYaw, Tc::Haptic::AxisPitch, Tc::Haptic::AxisRoll);
    }
}
#endif //RCUNITTOOLS_TCCONFIG_H
