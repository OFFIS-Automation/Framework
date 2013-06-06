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

#ifndef RCUNITHELP_H
#define RCUNITHELP_H

#include <QString>
#include <QMap>
#include <QVariant>
#include <QStringList>
#include "../telecontrol/TcConfig.h"

struct RcUnitHelp
{
    QString desc;
    QString server;
    struct Method
    {
        QString name;
        QString sig;
        QString shortDesc;
        QString longDesc;
    };
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

    struct Struct
    {
        QString name;
        QList<QString> members;
    };
    QList<Struct> structs;
    QList<Method> methods;
    QList<TcJostick> tcJoysticks;
    QList<TcButton> tcButtons;
    QVariantMap constants;
    bool hasHaptic;
    double hapticSensitivity;
    double hapticForceFactor;
};



#endif // RCUNITHELP_H