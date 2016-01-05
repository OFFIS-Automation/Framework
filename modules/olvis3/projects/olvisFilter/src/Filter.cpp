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

#include <core/Filter.h>
#include "UserFilterPrivate.h"
#include "PortData.h"
#include <filter/Port.h>
#include <stdexcept>

void Filter::initialize(int id, const QString &name, int processorId)
{
    mId = id;
    mName = name;
    mProcessorId = processorId;
    mInitialized = true;
}


Filter::Filter()
{
    mProcessorId = 0;
    mId = 0;
    mInitialized = false;
}


void Filter::connect(const QString &portId, Filter *targetFilter, const QString &targetPortId)
{
    OutputPort* source = getOutput(portId);
    if(!source)
        throw std::runtime_error(tr("Core error: could not connect filters: source port not found").toStdString());
    InputPort* target = targetFilter->getInput(targetPortId);
    if(!target)
        throw std::runtime_error(tr("Core error: could not connect filters: target port not found").toStdString());
    source->addTarget(target);
    target->setSource(this, source);
}

void Filter::connect(const QString &portId, PortListener *listener)
{
    OutputPort* source = getOutput(portId);
    if(!source)
        throw std::runtime_error(tr("Core error: could not connect listener: source port not found").toStdString());
    source->addTarget(listener);
}

void Filter::disconnect(const QString &portId, Filter *targetFilter, const QString &targetPortId)
{
    OutputPort* source = getOutput(portId);
    if(!source)
        throw std::runtime_error(tr("Core error: could not connect filters: source port not found").toStdString());
    InputPort* target = targetFilter->getInput(targetPortId);
    if(!target)
        throw std::runtime_error(tr("Core error: could not connect filters: target port not found").toStdString());
    source->removeTarget(target);
    target->removeSource();
}

void Filter::disconnect(const QString &portId, PortListener *listener)
{
    OutputPort* source = getOutput(portId);
    if(!source)
        return;
    source->removeTarget(listener);
}

bool Filter::source(const QString &portId, QObject *&filter, QString &portName) const
{
    InputPort* input = getInput(portId);
    if(!input)
        return false;
    filter = input->getSourceFilter();
    if(!filter)
        return false;
    OutputPort* p = input->getSourcePort();
    if(p)
        portName = p->name();
    return true;
}

bool Filter::hasSource(const QString &inputName) const
{
    QObject* f;
    QString s;
    return source(inputName, f, s);
}

QVariant Filter::portValue(const QString &name)
{
    InputPort* input = getInput(name);
    if(input)
        return input->peekValue();
    OutputPort* output = getOutput(name);
    if(output)
        return output->lastValue();
    return QVariant();
}

QString Filter::portValueToString(const QString &portName, const QVariant& value) const
{
    Port* p = getPort(portName);
    if(!p)
        return QString();
    return p->toString(p->constrainedValue(value));
}

QVariant Filter::portValueFromString(const QString &portName, const QString &value) const
{
    Port* p = getPort(portName);
    if(!p)
        return QVariant();
    return p->constrainedValue(p->fromString(value));
}

QVariant Filter::constrainedPortValue(const QString &portName, const QVariant &value) const
{
    Port* p = getPort(portName);
    if(!p)
        return QVariant();
    return p->constrainedValue(value);
}

QVariant Filter::portValueToSimpleType(const QString &portName, const QVariant &value) const
{
    Port* p = getPort(portName);
    if(!p)
        return QVariant();
    return p->toSimpleType(value);
}
