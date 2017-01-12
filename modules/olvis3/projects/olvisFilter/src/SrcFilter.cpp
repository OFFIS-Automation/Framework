// OFFIS Automation Framework
// Copyright (C) 2013-2017 OFFIS e.V.
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


#include <core/SrcFilter.h>
#include <filter/UserFilter.h>
#include <core/FilterInfo.h>
#include "UserFilterPrivate.h"
#include "PortData.h"
#include <core/Tracer.h>
#include <QDebug>

#include <stdexcept>

#include "OlvisSensorProvider.h"

SrcFilter::SrcFilter(UserFilter *userFilter)
{
    mUserFilter = userFilter;
}

SrcFilter::~SrcFilter()
{
    if(mInitialized)
        mUserFilter->deinitialize();
    delete mUserFilter;
#ifndef NO_SENSOR_SYSTEM
    qDeleteAll(mDataProvider);
#endif
}

void SrcFilter::initialize(int id, const QString &name, int processorId)
{
    Filter::initialize(id, name, processorId);
    mUserFilter->initialize();
    foreach(InputPort* port, mUserFilter->d->inputs.values())
    {
        QObject::connect(port, SIGNAL(portValueChanged(QString,QVariant)), this, SLOT(onUserPortValueChanged(QString,QVariant)), Qt::DirectConnection);
    }
    setName(name);
}

InputPort* SrcFilter::getInput(const QString &name) const
{
    return mUserFilter->d->inputs.value(name, 0);
}

OutputPort* SrcFilter::getOutput(const QString &name) const
{
    return mUserFilter->d->outputs.value(name, 0);
}

Port *SrcFilter::getPort(const QString &name) const
{
    return mUserFilter->d->ports.value(name, 0);
}

void SrcFilter::setName(const QString &name)
{
    mName = name;
#ifndef NO_SENSOR_SYSTEM
    foreach(OutputPort* port, mUserFilter->d->outputs)
    {
        OlvisSensorProvider* oldProvider = mDataProvider.value(port, 0);
        if(oldProvider)
        {
            port->removeTarget(oldProvider);
            delete oldProvider;
        }
        OlvisSensorProvider* newProvider = new OlvisSensorProvider(name, port->name());
        port->addTarget(newProvider);
        mDataProvider[port] = newProvider;
    }
#endif
}

FilterInfo SrcFilter::info() const
{
    FilterInfo info;
    info.id = mId;
    info.name = mName;
    info.processorId = mProcessorId;
    info.typeInfo = mUserFilter->d->info;
    // update input constraints
    for(int i = 0; i< info.typeInfo.inputs.size(); i++)
    {
        PortInfo& pInfo = info.typeInfo.inputs[i];
        pInfo.constraints = getPort(pInfo.name)->getInfo().constraints;
    }

    info.typeInfo.plugin = pluginName();
    info.typeInfo.uid = pluginName() + ":" + info.typeInfo.name;
    return info;
}

void SrcFilter::setInput(const QString &name, const QVariant &value)
{
    QMutexLocker lock(&mUpdateMutex);
    InputPort* port = getInput(name);
    if(!port)
        return;
    if(mExecMutex.tryLock())
    {
        QVariant val = port->setDefaultValue(value);
        emit portValueChanged(id(), name, val);
        mExecMutex.unlock();
        return;
    }
    mBufferedPortUpdates[port] = value;
}


void SrcFilter::startup()
{
    try {
        mTracer = new SubTracer(name());
        foreach(OutputPort* output, mUserFilter->d->outputs)
        {
            output->setTracer(mTracer);
        }

        mUserFilter->start();
        mTracer->trace(Tracer::FilterStarted);
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error(e.what());
    }
    catch(...)
    {
        throw std::runtime_error(tr("Unknown error").toStdString());
    }
}

void SrcFilter::shutdown()
{
    mUserFilter->stop();
    foreach(OutputPort* output, mUserFilter->d->outputs)
    {
        output->setTracer(0);
    }
    mTracer->setId(0);
    mTracer->trace(Tracer::FilterStopped);
    delete mTracer;
}

bool SrcFilter::hasOutputs() const
{
    return !(mUserFilter->d->outputs.empty());
}

bool SrcFilter::execute(int runId)
{
    QMutexLocker lock(&mExecMutex);

    bool valid = false;
    try
    {
        QMutableMapIterator<QString, InputPort*> inputs(mUserFilter->d->inputs);
        // execution is valid if at least one output was set
        bool updated = false;
        bool hasConnections = false;
        while(inputs.hasNext())
        {
            InputPort* input = inputs.next().value();
            if(input->isConnected())
            {
                hasConnections = true;
                if(input->isUpdated())
                {
                    updated = true;
                    break;
                }
            }
        }
        if(hasConnections && !updated)
            return false;
        mTracer->setId(runId);
        mTracer->trace(Tracer::FilterActivated);
        mUserFilter->execute();
        mTracer->trace(Tracer::FilterFinished);
        // empty output nodes are always successfull
        if(!hasOutputs())
            valid = true;
        QMutableMapIterator<QString, OutputPort*> outputs(mUserFilter->d->outputs);
        // execution is valid if at least one output was set
        while(outputs.hasNext())
        {
            outputs.next();
            OutputPort* output = outputs.value();
            output->finish();
            //if(!output->isNormalType())
            valid |= output->hasFired();
            output->resetFired();
        }
        // reset single shot input ports
        inputs.toFront();
        while(inputs.hasNext())
        {
            InputPort* input = inputs.next().value();
            if(input->mode() == SingleShotPortMode)
                input->setValue(QVariant());
            input->unsetChanged();
        }
    }
    catch(const std::exception& e)
    {
        processBufferedUpdates();
        throw std::runtime_error(e.what());
    }

    catch(...)
    {
        processBufferedUpdates();
        throw std::runtime_error(tr("Unknown error").toStdString());
    }
    processBufferedUpdates();
    return valid;
}

void SrcFilter::processBufferedUpdates()
{
    QMutexLocker lock(&mUpdateMutex);
    QMutableMapIterator<InputPort*, QVariant> updatedPortValues(mBufferedPortUpdates);
    while(updatedPortValues.hasNext())
    {
        updatedPortValues.next();
        InputPort* port = updatedPortValues.key();
        QVariant val = port->setDefaultValue(updatedPortValues.value());
        emit portValueChanged(id(), port->name(), val);
    }
    mBufferedPortUpdates.clear();
}

void SrcFilter::onUserPortValueChanged(const QString& portId, const QVariant& value)
{
    emit portValueChanged(id(), portId, value);
}

void SrcFilter::onUserConstraintChanged(const QString &portId)
{
    emit constraintsUpdated(id(), portId);
}

