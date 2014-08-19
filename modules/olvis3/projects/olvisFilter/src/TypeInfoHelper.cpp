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

#include <filter/FilterTypeInfoHelper.h>

FilterTypeInfoHelper::FilterTypeInfoHelper(const FilterTypeInfo &typeInfo) : mInfo(typeInfo)
{
}

const PortInfo& FilterTypeInfoHelper::getPort(const QList<PortInfo> &source, const QString &portId)
{
    static PortInfo dummy;
    QListIterator<PortInfo> iter(source);
    while(iter.hasNext())
    {
        const PortInfo& port = iter.next();
        if(port.name == portId)
            return port;
    }
    return dummy;
}

int FilterTypeInfoHelper::portType(const QString &id) const
{
    const PortInfo& input = getInputPort(id);
    if(input.isValid())
        return input.type;

    const PortInfo& output = getOutputPort(id);
    if(output.isValid())
        return output.type;

    return -1;
}

const PortInfo& FilterTypeInfoHelper::getInputPort(const QString &portId) const
{
    return getPort(mInfo.inputs, portId);
}

const PortInfo& FilterTypeInfoHelper::getOutputPort(const QString &portId) const
{
    return getPort(mInfo.outputs, portId);
}

bool FilterTypeInfoHelper::hasInputPort(const QString &id) const
{
    return getInputPort(id).isValid();
}

bool FilterTypeInfoHelper::hasOutputPort(const QString &id) const
{
    return getOutputPort(id).isValid();
}

bool FilterTypeInfoHelper::isMode(const QString &id, InputPortMode mode) const
{
    const PortInfo& info = getInputPort(id);
    if(info.isValid())
        return info.mode == mode;
    return false;
}

bool FilterTypeInfoHelper::hasPort(const QString &id) const
{
    return hasInputPort(id) || hasOutputPort(id);
}

bool FilterTypeInfoHelper::canConnect(const QString &portId, const PortInfo& info, QString &warning) const
{
    warning.clear();
    const PortInfo& port = getInputPort(portId);
    if(!port.isValid())
        return false;
    if(!port.isArray && info.isArray)
        warning = tr("Connecting list port to single port: only the first element of the list will be used\n");
    if(port.isArray && !info.isArray)
        warning = tr("Connecting single port to list port: value will be converted into a one-element list\n");
    if(port.type == info.type)
        return true;
    if(port.compatibleTypes.contains(info.type))
        return true;
    if(port.partlyCompatibleTypes.contains(info.type))
    {
        warning += port.partlyCompatibleTypes[info.type];
        return true;
    }
    warning.clear();
    return false;
}
