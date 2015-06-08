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


#ifndef TELECONTROLCONFIG_H
#define TELECONTROLCONFIG_H

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

    struct TcMove
    {
        QString name;
        int numSensitivityScalingTicks;
        int deadMansButton;
        double sensitivity;
        double forceScaling;
        QStringList axeNames;
        QList<int> analogDOFs;
        QList<bool> inverts;
        TcMove() : numSensitivityScalingTicks(10) {}
    };

    QString tcDeviceName;

    QList<TcMove> tcGamepadMoves;
    QList<TcMove> tcConnexionMoves;
    QList<TcMove> tcHapticMoves;
    QList<TcButton> tcButtonMethods;
};

inline bool isGamepadButton(int button)
{
    return button >= Tc::Gamepad::FirstButton && button <= Tc::Gamepad::LastButton;
}

inline bool isConnexionButton(int button)
{
    return button >= Tc::Connexion::FirstButton && button <= Tc::Connexion::LastButton;
}

inline bool isVirtualButton(int button)
{
    return button >= Tc::Virtual::FirstButton && button <= Tc::Virtual::LastButton;
}

inline bool isHapticButton(int button)
{
    return button >= Tc::Haptic::FirstButton && button <= Tc::Haptic::LastButton;
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

inline QDataStream& operator>>(QDataStream& stream, TelecontrolConfig::TcMove& move)
{
    stream >> move.name;
    stream >> move.deadMansButton;
    stream >> move.sensitivity;
    stream >> move.forceScaling;
    stream >> move.axeNames;
    stream >> move.analogDOFs;
    stream >> move.inverts;
    return stream;
}

inline QDataStream& operator<<(QDataStream& stream, const TelecontrolConfig::TcMove& move)
{
    stream << move.name;
    stream << move.deadMansButton;
    stream << move.sensitivity;
    stream << move.forceScaling;
    stream << move.axeNames;
    stream << move.analogDOFs;
    stream << move.inverts;
    return stream;
}

inline QDataStream& operator>>(QDataStream& stream, TelecontrolConfig& tc)
{
    stream >> tc.unitName;
    stream >> tc.tcDeviceName;
    stream >> tc.tcGamepadMoves;
    stream >> tc.tcConnexionMoves;
    stream >> tc.tcHapticMoves;
    stream >> tc.tcButtonMethods;
    return stream;
}

inline QDataStream& operator<<(QDataStream& stream, const TelecontrolConfig& tc)
{
    stream << tc.unitName;
    stream << tc.tcDeviceName;
    stream << tc.tcGamepadMoves;
    stream << tc.tcConnexionMoves;
    stream << tc.tcHapticMoves;
    stream << tc.tcButtonMethods;
    return stream;
}

#endif // TELECONTROLCONFIG_H
