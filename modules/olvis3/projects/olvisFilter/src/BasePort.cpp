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

#include "BasePort.h"

BasePort::BasePort(int typeId, const QString& typeName, bool isArray, const QString& name)
{
    mInfo.mode = RegularPortMode;
    mInfo.type = typeId;
    mInfo.typeName = typeName;
    mInfo.name = name;
    mInfo.isArray = isArray;
    mInfo.isMainType = false;
}

void BasePort::setDesc(const QString &desc)
{
    mInfo.desc = desc;
}

void BasePort::setName(const QString &name)
{
    mInfo.name = name;
}

void BasePort::setContraint(const QByteArray &key, const QVariant &value)
{
    mInfo.constraints[key] = value;
}

void BasePort::addCompatiblePort(int typeId, bool warnOnConnect, const QString &warnMsg)
{
    if(warnOnConnect)
        mInfo.partlyCompatibleTypes[typeId] = warnMsg;
    else
        mInfo.compatibleTypes.insert(typeId);
}

void BasePort::setIsMainType(bool main)
{
    mInfo.isMainType = main;
}

void BasePort::setMode(InputPortMode mode)
{
    mInfo.mode = mode;
}

void BasePort::setIcon(const QImage &icon)
{
    mInfo.icon = icon;
}
