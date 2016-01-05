// OFFIS Automation Framework
// Copyright (C) 2013-2016 OFFIS e.V.
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

class UserRcUnit;

class RcUnitBaseObserver
{
public:
    virtual void rcUnitStatusChanged(bool acquired) = 0;
};

class RCUNITS_EXPORT RcUnitBase : public HapticBaseEndpoint, public GamepadEndpoint
{
public:
    RcUnitBase();
    virtual ~RcUnitBase(){}

    virtual QMap<QString, QVariant> getConstants() const = 0;
    virtual QVariant call(const QByteArray& method, QList<QVariant> params) = 0;
    virtual RcUnitHelp getHelp() const = 0;
    virtual UserRcUnit* rcUnit() = 0;
    virtual QString name() const = 0;
    virtual void acquire() = 0;
    virtual void release() = 0;
    virtual void stop() = 0;
    virtual bool acquired() = 0;
    virtual void setObserver(RcUnitBaseObserver* observer) = 0;
    virtual QVariantList getFlags() = 0;
    virtual TelecontrolConfig telecontrolConfig() const { return getHelp(); }
    virtual void updateTelecontrolAssignment(const QString &telecontrolDeviceName) = 0;
};

#endif // RCUNITBASE_H
