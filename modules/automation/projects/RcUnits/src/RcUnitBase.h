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
#include "GamepadEndpoint.h"
#include "HapticBaseEndpoint.h"

class RCUNITS_EXPORT RcUnitBase : public HapticBaseEndpoint, public GamepadEndpoint
{
public:
    RcUnitBase();
    virtual ~RcUnitBase(){}

    virtual QMap<QString, QVariant> getConstants() const = 0;
    virtual QVariant call(const QByteArray& method, QList<QVariant> params) = 0;
    virtual RcUnitHelp getHelp() const = 0;
    virtual QObject* rcUnit() = 0;
    virtual QString name() const = 0;
    virtual TelecontrolConfig telecontrolConfig() const { return getHelp(); }
};

#endif // RCUNITBASE_H
