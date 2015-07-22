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

#include <filter/Port.h>
#include "PortData.h"

Port::Port(int id, const QString &name, bool isMainType) : d(new PortData())
{
    d->in = 0;
    d->out = 0;
    d->info.type = id;
    d->info.isArray = false;
    d->info.typeName = name;
    d->info.isMainType = isMainType;
    d->info.mode = RegularPortMode;
    d->info.visibility = UserPortVisiblility;
    d->info.name = name;
}

void Port::setName(const QString &name)
{
    d->info.name = name;
    if(d->info.desc.isEmpty())
        setDesc(name);
}

void Port::setDesc(const QString &desc)
{
    d->info.desc = desc;
}

void Port::setIcon(const QImage &icon)
{
    d->info.icon = icon;
}

void Port::setMode(InputPortMode mode)
{
    d->info.mode = mode;
}

void Port::setVisibility(PortVisibility visibility)
{
    d->info.visibility = visibility;
}

PortInfo Port::getInfo() const
{
    return d->info;
}

const QString& Port::name() const
{
    return d->info.name;
}

InputPortMode Port::mode() const
{
    return d->info.mode;
}
void Port::setConstraint(const QByteArray &key, const QVariant &value)
{
    if(d->info.constraints.value(key) != value)
    {
        d->info.constraints[key] = value;
        if(d->in)
            d->in->newConstraint();
    }
}

void Port::recheck()
{
    if(!d->in)
        return;
    d->in->recheck();
}

void Port::addChoiceRaw(const QVariant &value, const QString &name)
{
   d->choiceValues.append(value);
   d->choiceNames.append(name);
   setConstraint("choices", d->choiceValues);
   setConstraint("choiceNames", d->choiceNames);
}


// Input port
bool Port::hasValue()
{
    if(!d->in)
        return false;
    return d->in->hasValue();
}

bool Port::isUpdated()
{
    if(!d->in)
        return false;
    return d->in->isUpdated();
}

bool Port::hasChanged()
{
    if(!d->in)
        return false;
    return d->in->hasChanged();
}

QVariant Port::getRawValue()
{
    if(!d->in)
        return QVariant();
    return d->in->getRawValue();
}

QVariant Port::setDefaultValueRaw(const QVariant &var)
{
    d->info.defaultValue = var;
    if(!d->in)
        return var;
    QVariant var2 = d->in->setDefaultValue(var);
    return var2;
}

void Port::addCompatiblePort(int typeId)
{
    d->info.compatibleTypes.insert(typeId);
}

void Port::addCompatiblePort(int typeId, const QString &warningMsg)
{
    d->info.partlyCompatibleTypes[typeId] = warningMsg;
}

// output port
void Port::sendRaw(const QVariant &value)
{
    if(!d->out)
        return;
    d->out->send(value);
}

void Port::setTypeName(const QString &typeName)
{
    d->info.typeName = typeName;
}

void Port::setType(int type)
{
    d->info.type = type;
}

