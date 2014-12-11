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

#include <filter/UserFilter.h>
#include "UserFilterPrivate.h"
#include "PortData.h"

UserFilter::UserFilter()
{
    d = new UserFilterPrivate();
}

UserFilter::~UserFilter()
{
    qDeleteAll(d->inputs);
    qDeleteAll(d->outputs);
    delete d;
}

void UserFilter::setName(const QString &name)
{
    d->info.name = name;
}

void UserFilter::setDesc(const QString &desc)
{
    d->info.desc = desc;
}

void UserFilter::setGroup(const QString &group)
{
    d->info.group = group.split("/", QString::SkipEmptyParts);
}

void UserFilter::addInputPort(Port &port)
{
    if(d->ports.contains(port.name()))
        return;
    d->inputs[port.name()] = new InputPort(port);
    d->ports[port.name()] = &port;
    d->info.inputs.append(port.getInfo());
}

void UserFilter::addInputListPort(Port& port)
{
    if(d->ports.contains(port.name()))
        return;
    d->inputs[port.name()] = new InputListPort(port);
    d->ports[port.name()] = &port;
    d->info.inputs.append(port.getInfo());
}

void UserFilter::addOutputPort(Port &port)
{
    if(d->ports.contains(port.name()))
        return;
    d->outputs[port.name()] = new OutputPort(port);
    d->ports[port.name()] = &port;
    d->info.outputs.append(port.getInfo());
}

void UserFilter::addOutputListPort(Port &port)
{
    if(d->ports.contains(port.name()))
        return;
    d->outputs[port.name()] = new OutputListPort(port);
    d->ports[port.name()] = &port;
    d->info.outputs.append(port.getInfo());
}
