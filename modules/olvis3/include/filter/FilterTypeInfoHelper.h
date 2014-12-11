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

#ifndef EXTENDEDFILTERTYPEINFO_H
#define EXTENDEDFILTERTYPEINFO_H

#include "../core/FilterTypeInfo.h"
#include "FilterBaseGlobal.h"

class FILTERBASESHARED_EXPORT FilterTypeInfoHelper : public QObject
{
public:
    FilterTypeInfoHelper(const FilterTypeInfo& typeInfo);
    const PortInfo& getInputPort(const QString& portId) const;
    const PortInfo& getOutputPort(const QString& portId) const;
    bool canConnect(const QString& port, const PortInfo& info, QString& warning) const;

    bool hasInputPort(const QString& id) const;
    bool hasOutputPort(const QString& id) const;
    bool hasPort(const QString& id) const;
    int portType(const QString& id) const;
    bool isMode(const QString& id, InputPortMode mode) const;
protected:
    static const PortInfo& getPort(const QList<PortInfo>& source, const QString& portId);
    const FilterTypeInfo& mInfo;
};

#endif // EXTENDEDFILTERTYPEINFO_H
