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


#ifndef RCUNITTYPES_H
#define RCUNITTYPES_H

#include <QString>

#include <QList>

enum UserRcUnitType
{
    BaseRcUnitType,
    HwRcUnitType,
    RobotRcUnitType
};

struct RcFlagDefinition {
    QString name;
    QString unit;
    int decimalPlaces;
    RcFlagDefinition() : decimalPlaces(2) {}
    RcFlagDefinition(QString name) : name(name), decimalPlaces(2) {}
    RcFlagDefinition(QString name, QString unit)  : name(name), unit(unit), decimalPlaces(2) {}
    RcFlagDefinition(QString name, int decimalPlaces) : name(name), decimalPlaces(decimalPlaces) {}
    RcFlagDefinition(QString name, QString unit, int decimalPlaces)
        : name(name), unit(unit), decimalPlaces(decimalPlaces) {}
};

typedef QList<RcFlagDefinition> RcFlagDefinitions;

#endif // RCUNITTYPES_H
