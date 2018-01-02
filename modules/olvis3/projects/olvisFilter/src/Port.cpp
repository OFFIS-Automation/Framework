// OFFIS Automation Framework
// Copyright (C) 2013-2018 OFFIS e.V.
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

#include <filter/Port.h>
#include "PortData.h"

Port::Port(int id, const QString &name, bool isMainType) : portData(new PortData())
{
    portData->in = 0;
    portData->out = 0;
    portData->info.type = id;
    portData->info.isArray = false;
    portData->info.typeName = name;
    portData->info.isMainType = isMainType;
    portData->info.mode = RegularPortMode;
    portData->info.visibility = UserPortVisiblility;
    portData->info.name = name;
}

void Port::setName(const QString &name)
{
    portData->info.name = name;
}

void Port::setDesc(const QString &desc)
{
    portData->info.desc = desc;
}

void Port::setIcon(const QImage &icon)
{
    portData->info.icon = icon;
}

void Port::setMode(InputPortMode mode)
{
    portData->info.mode = mode;
}

void Port::setVisibility(PortVisibility visibility)
{
    portData->info.visibility = visibility;
}

bool Port::isConnetected()
{
    if(portData->in)
        return portData->in->isConnected();
    if(portData->out)
        return portData->out->hasTargets();
    return false;
}

bool Port::isOverlayed()
{
    if(portData->out)
        return portData->out->isOverlayed();
    return false;
}

PortInfo Port::getInfo() const
{
    return portData->info;
}

const QString& Port::name() const
{
    return portData->info.name;
}

InputPortMode Port::mode() const
{
    return portData->info.mode;
}

void Port::setConstraint(const QByteArray &key, const QVariant &value)
{
    if(portData->info.constraints.value(key) != value)
    {
        portData->info.constraints[key] = value;
        if(portData->in)
            portData->in->newConstraint();
    }
}

void Port::recheck()
{
    if(!portData->in)
        return;
    portData->in->recheck();
}

void Port::addChoiceRaw(const QVariant &value, const QString &name)
{
   portData->choiceValues.append(value);
   portData->choiceNames.append(name);
   setConstraint("choices", portData->choiceValues);
   setConstraint("choiceNames", portData->choiceNames);
}

// Input port
bool Port::hasValue()
{
    if(!portData->in)
        return false;
    return portData->in->hasValue();
}

bool Port::isUpdated()
{
    if(!portData->in)
        return false;
    return portData->in->isUpdated();
}

bool Port::hasChanged()
{
    if(!portData->in)
        return false;
    return portData->in->hasChanged();
}

QVariant Port::getRawValue()
{
    if(!portData->in)
        return QVariant();
    return portData->in->getRawValue();
}

QVariant Port::setDefaultValueRaw(const QVariant &var)
{
    portData->info.defaultValue = var;
    if(!portData->in)
        return var;
    return portData->in->setDefaultValue(var);
}

void Port::addCompatiblePort(int typeId)
{
    portData->info.compatibleTypes.insert(typeId);
}

void Port::addCompatiblePort(int typeId, const QString &warningMsg)
{
    portData->info.partlyCompatibleTypes[typeId] = warningMsg;
}

// output port
void Port::sendRaw(const QVariant &value)
{
    if(!portData->out)
        return;
    portData->out->send(value);
}

void Port::setTypeName(const QString &typeName)
{
    portData->info.typeName = typeName;
}

void Port::setType(int type)
{
    portData->info.type = type;
}

