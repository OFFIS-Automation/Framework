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
/*
inline QDataStream& operator>>(QDataStream& stream, Tc::Connexion::Joystick& joystick)
{

    int id;
    stream >> id;
    joystick = (Tc::Connexion::Joystick)id;
    if(!Tc::joysticks().contains(joystick))
        joystick = Tc::NoJoystick;
    return stream;
}

inline QDataStream& operator>>(QDataStream& stream, Tc::Haptic::Axis& axis)
{

    int id;
    stream >> id;
    axis = (Tc::Haptic::Axis)id;
    if(!Tc::hapticAxis().contains(axis))
        axis = Tc::NoAxis;
    return stream;
}
*/

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
//    stream >> move.forceScaling;
    stream >> move.axeNames;
    stream >> move.analogDOFs;
//    stream >> move.axes;
    stream >> move.inverts;
    return stream;
}

inline QDataStream& operator<<(QDataStream& stream, const TelecontrolConfig::TcMove& move)
{
    stream << move.name;
    stream << move.deadMansButton;
    stream << move.sensitivity;
//    stream << move.forceScaling;
    stream << move.axeNames;
    stream << move.analogDOFs;
//    stream << move.axes;
    stream << move.inverts;
    return stream;
}

inline QDataStream& operator>>(QDataStream& stream, TelecontrolConfig& tc)
{
    stream >> tc.unitName;
    stream >> tc.tcDeviceName;
    stream >> tc.tcGamepadMoves;
    stream >> tc.tcButtonMethods;
    stream >> tc.tcHapticMoves;
    return stream;
}

inline QDataStream& operator<<(QDataStream& stream, const TelecontrolConfig& tc)
{
    stream << tc.unitName;
    stream << tc.tcDeviceName;
    stream << tc.tcGamepadMoves;
    stream << tc.tcButtonMethods;
    stream << tc.tcHapticMoves;
    return stream;
}

#endif // TELECONTROLCONFIG_H
