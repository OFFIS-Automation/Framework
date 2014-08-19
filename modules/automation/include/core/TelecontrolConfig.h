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


#ifndef GAMEPADCONFIG_H
#define GAMEPADCONFIG_H

#include <QString>
#include <QStringList>
#include "../telecontrol/TcConfig.h"

struct TelecontrolConfig
{
    QString unitName;
    struct TcButton
    {
        QString name;
        int buttonId;
        bool toggleMode;
    };

    struct TcJostick
    {
        QString name;
        int deadMansButton;
        double sensitivity;
        QStringList axeNames;
        QList<Tc::Joystick> joysticks;
        QList<bool> inverts;
    };

    QList<TcJostick> tcJoysticks;
    QList<TcButton> tcButtons;

    bool hasHaptic;
    double hapticSensitivity;
    double hapticForceFactor;
};

inline QDataStream& operator>>(QDataStream& stream, Tc::Joystick& joystick)
{

    int id;
    stream >> id;
    joystick = (Tc::Joystick)id;
    if(!Tc::joysticks().contains(joystick))
        joystick = Tc::NoJoystick;
    return stream;
}


inline QDataStream& operator>>(QDataStream& stream, TelecontrolConfig::TcButton& button)
{
    stream >> button.name;
    stream >> button.buttonId;
    stream >> button.toggleMode;
    return stream;
}

inline QDataStream& operator<<(QDataStream& stream, const TelecontrolConfig::TcButton& button)
{
    stream << button.name;
    stream << button.buttonId;
    stream << button.toggleMode;
    return stream;
}

inline QDataStream& operator>>(QDataStream& stream, TelecontrolConfig::TcJostick& joystick)
{
    stream >> joystick.name;
    stream >> joystick.deadMansButton;
    stream >> joystick.sensitivity;
    stream >> joystick.axeNames;
    stream >> joystick.joysticks;
    stream >> joystick.inverts;
    return stream;
}

inline QDataStream& operator<<(QDataStream& stream, const TelecontrolConfig::TcJostick& joystick)
{
    stream << joystick.name;
    stream << joystick.deadMansButton;
    stream << joystick.sensitivity;
    stream << joystick.axeNames;
    stream << joystick.joysticks;
    stream << joystick.inverts;
    return stream;
}


inline QDataStream& operator>>(QDataStream& stream, TelecontrolConfig& tc)
{
    stream >> tc.unitName;
    stream >> tc.tcJoysticks;
    stream >> tc.tcButtons;
    stream >> tc.hasHaptic;
    stream >> tc.hapticSensitivity;
    stream >> tc.hapticForceFactor;
    return stream;
}

inline QDataStream& operator<<(QDataStream& stream, const TelecontrolConfig& tc)
{
    stream << tc.unitName;
    stream << tc.tcJoysticks;
    stream << tc.tcButtons;
    stream << tc.hasHaptic;
    stream << tc.hapticSensitivity;
    stream << tc.hapticForceFactor;
    return stream;
}



#endif // GAMEPADCONFIG_H
