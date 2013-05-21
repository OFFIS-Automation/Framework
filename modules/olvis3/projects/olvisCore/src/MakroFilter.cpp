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

#include "MakroFilter.h"

#include <core/FilterInfo.h>
#include <core/InputPort.h>
#include <core/OutputPort.h>
#include <QDebug>
#include <filter/Port.h>

MakroFilter::MakroFilter(bool instance)
    : mIsInstance(instance)
{
}

MakroFilter::~MakroFilter()
{
    if(mIsInstance)
        qDeleteAll(mFilters);
    qDeleteAll(mInputs);
    qDeleteAll(mOutputs);
}

void MakroFilter::initialize(int id_, const QString &name, int processorId)
{
    Filter::initialize(id_, name, processorId);
    foreach(Filter* filter, mFilters)
        filter->initialize(0,QString(), id());
}

FilterGroupInfo MakroFilter::groupInfo() const
{
    FilterGroupInfo info;
    info.id = id();
    info.name = name();
    QMapIterator<QString, Input*> inputs(mInputs);
    while(inputs.hasNext())
    {
        inputs.next();
        Input* input = inputs.value();
        FilterGroupPort port;
        port.name = inputs.key();
        port.filterId = input->filter->id();
        port.port = input->port->getInfo();
        port.parentId = id();
        info.inputs << port;
    }
    QMapIterator<QString, Output*> outputs(mOutputs);
    while(outputs.hasNext())
    {
        outputs.next();
        Output* output = outputs.value();
        FilterGroupPort port;
        port.name = outputs.key();
        port.filterId = output->filter->id();
        port.port = output->port->getInfo();
        port.parentId = id();
        info.outputs << port;
    }
    return info;
}

FilterInfo MakroFilter::info() const
{
    FilterInfo info;
    info.id = mId;
    info.name = mName;
    info.processorId = mProcessorId;
    info.typeInfo.plugin = tr("MakroFilter");
    info.typeInfo.desc = tr("User defined makro filter");
    info.typeInfo.group = QStringList("user") << "makro";
    info.typeInfo.name = mName;
    info.typeInfo.uid = ":Makro:" + mName;
    QMapIterator<QString, Input*> inputs(mInputs);
    while(inputs.hasNext())
    {
        inputs.next();
        PortInfo pinfo = inputs.value()->iport->getInfo();
        pinfo.name = inputs.key();
        info.typeInfo.inputs.append(pinfo);
    }
    QMapIterator<QString, Output*> outputs(mOutputs);
    while(outputs.hasNext())
    {
        outputs.next();
        PortInfo pinfo = outputs.value()->oport->getInfo();
        pinfo.name = outputs.key();
        info.typeInfo.outputs.append(pinfo);
    }
    return info;
}

bool MakroFilter::hasOutputs() const
{
    return mOutputs.empty();
}

void MakroFilter::startup()
{
    QMutableListIterator<Filter*> filters(mFilters);
    try
    {
        while(filters.hasNext())
            filters.next()->startup();
    }
    catch(...)
    {
        while(filters.hasPrevious())
        {
            try
            {
                filters.previous()->shutdown();
            } catch( ... ) {}
        }
        throw;
    }
}

void MakroFilter::shutdown()
{
    QMutableListIterator<Filter*> filters(mFilters);
    while(filters.hasNext())
    {
        try
        {
            filters.next()->shutdown();
        } catch( ... ) {}
    }
}

bool MakroFilter::execute(int runId)
{
    bool working = false;
    QMutableListIterator<Filter*> filters(mFilters);
    while(filters.hasNext())
    {
        Filter* filter = filters.next();
        if(!filter->execute(runId)) // one filter failed or is finished, skip following filters
            break;
        if(filter->hasOutputs()) // only filter with outputs have impact to the wokring flag of the makro filter
            working = true;
    }
    return working;
}

InputPort *MakroFilter::getInput(const QString &name) const
{
    Input* input = mInputs.value(name, 0);
    if(!input)
        return 0;
    return input->iport;
}

void MakroFilter::setInput(const QString &port, const QVariant &value)
{
    Input* i = mInputs.value(port);
    if(!i)
        return;
    i->filter->setInput(i->realName, value);
}

OutputPort *MakroFilter::getOutput(const QString &name) const
{
    Output* o = mOutputs.value(name, 0);
    if(!o)
        return 0;
    return o->oport;
}

Port *MakroFilter::getPort(const QString &name) const
{
    Input* i = mInputs.value(name);
    if(i)
        return i->port;
    Output* o = mOutputs.value(name, 0);
    if(!o)
        return 0;
    return o->port;
}

bool MakroFilter::hasOutput(int filterId, const QString &portId) const
{
    foreach(Output* output,  mOutputs)
    {
        if(output->filter->id() == filterId && output->realName == portId)
            return true;
    }
    return false;
}

bool MakroFilter::hasOutput(const QString &name) const
{
    return mOutputs.contains(name);
}

void MakroFilter::createOutput(const QString &name, Filter *sourceFilter, const QString &portId)
{
    QMutexLocker lock(&mPortMutex);
    Output* output = new Output();
    output->filter = sourceFilter;
    output->realName = portId;
    output->oport = sourceFilter->getOutput(portId);
    output->port = sourceFilter->getPort(portId);
    mOutputs[name] = output;
}


void MakroFilter::removeOutput(Filter *sourceFilter, const QString &portId)
{
    QMutexLocker lock(&mPortMutex);
    QString name;
    foreach(Output* output,  mOutputs)
    {
        if(output->filter == sourceFilter && output->realName == portId)
        {
            name = mOutputs.key(output);
            break;
        }
    }
    if(!name.isEmpty())
        mOutputs.remove(name);
}

void MakroFilter::renameOutput(Filter *filter, const QString &portId, const QString &newName)
{
    removeOutput(filter, portId);
    createOutput(newName, filter, portId);
}

void MakroFilter::createInput(const QString &name, Filter *targetFilter, const QString &portId)
{
    QMutexLocker lock(&mPortMutex);
    Input* input = new Input();
    input->filter = targetFilter;
    input->realName = portId;
    input->iport = targetFilter->getInput(portId);
    input->port = targetFilter->getPort(portId);
    mInputs[name] = input;
    QObject::connect(targetFilter, SIGNAL(portValueChanged(int,QString,QVariant)), SLOT(onPortValueChanged(int,QString,QVariant)), Qt::DirectConnection);
}

void MakroFilter::removeInput(int filterId, QString portId)
{
    QMutexLocker lock(&mPortMutex);
    foreach(Input* input,  mInputs)
    {
        if(input->filter->id() == filterId && input->realName == portId)
        {
            input->filter->QObject::disconnect(this);
            mInputs.remove(mInputs.key(input));
            return;
        }
    }
}


bool MakroFilter::hasInput(int filterId, const QString& portId)
{
    foreach(Input* input,  mInputs)
    {
        if(input->filter->id() == filterId && input->realName == portId)
            return true;
    }
    return false;
}

void MakroFilter::onPortValueChanged(int filterId, const QString &portId, const QVariant &value)
{
    QMutexLocker lock(&mPortMutex);
    QMapIterator<QString, Input*> inputs(mInputs);
    while(inputs.hasNext())
    {
        inputs.next();
        Input* input = inputs.value();
        if(input->filter->id() == filterId && input->realName == portId)
        {
            emit portValueChanged(id(), inputs.key(), value);
            return;
        }
    }
}
