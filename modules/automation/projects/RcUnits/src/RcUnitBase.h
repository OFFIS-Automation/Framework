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

#ifndef RCUNITBASE_H
#define RCUNITBASE_H

#include <core/RcUnitHelp.h>
#include <QVariant>
#include "RcUnitsGlobal.h"
#include <telecontrol/HapticInterface.h>

class RCUNITS_EXPORT RcUnitBase : public HapticEndpoint
{
public:
    RcUnitBase();
    virtual ~RcUnitBase(){}

    virtual QMap<QString, QVariant> getConstants() = 0;
    virtual QVariant call(const QByteArray& method, QList<QVariant> params) = 0;
    virtual RcUnitHelp getHelp() = 0;
    virtual QObject* lolec() = 0;
    virtual QString name() = 0;
    virtual void connectGamepad(QObject* gamepad) = 0;
    virtual void disconnectGamepad(QObject* gamepad) = 0;
    virtual void updateSensitivity(const QString& unitName, double sensitivity, const QList<bool>& inverts) = 0;
    virtual bool isTelecontrolable() const = 0;
    virtual bool hasHapticInterface() const = 0;
    virtual void updateHapticSensitivity(double sensitivity, double forceFactor) = 0;
    virtual double hapticSensitivity() const = 0;
    virtual double hapticForceFactor() const = 0;
};

#endif // RCUNITBASE_H
