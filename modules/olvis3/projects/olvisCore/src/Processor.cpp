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

#include "Processor.h"
#include "ProcessorOutputPort.h"
#include "ProcessorInputPort.h"
#include "ProcessorTriggerInterface.h"
#include <core/Tracer.h>
#include <core/PortListener.h>
#include <core/Filter.h>
#include <Notifications.h>
#include <QDebug>
#include <stdexcept>

Processor::Processor(int id, const QString& name, Priority priority)
{
    mPaused = false;
    mPauseOnStart = false;
    mInfo.id = id;
    mInfo.priority = priority;
    mInfo.pausedStartup = false;
    mInfo.ignoreTrigger = false;
    setName(name);
    mHasSource = false;
    mRunId = 0;
    mLoopId = 0;
}

Processor::~Processor()
{
    stop();
    wait();
}


void Processor::setProcessorPriority(QThread::Priority priority)
{
    if(isRunning())
        setPriority(priority);
    mInfo.priority = priority;
}

void Processor::setStartupMode(bool pausedStartup)
{
    mInfo.pausedStartup = pausedStartup;
}

void Processor::setTriggerMode(bool ignoreTrigger)
{
    mInfo.ignoreTrigger = ignoreTrigger;
}

void Processor::setStopMode(bool stopOnNoOutputGenerated)
{
    mInfo.stopOnNoOutput = stopOnNoOutputGenerated;
}

void Processor::setName(const QString& name)
{
    mInfo.name = name;
    setObjectName("OlvisProcessor: " + name);
}

void Processor::createOutput(const QString &name, Filter *sourceFilter, const QString &portId)
{
    ProcessorOutputPort* port = new ProcessorOutputPort(name, sourceFilter->id(), portId);
    mOutputs.append(port);
    sourceFilter->connect(portId, port);
    foreach(ProcessingElement* target, targets())
    {
        target->addPort(port);
    }
}

void Processor::removeOutput(Filter *sourceFilter, const QString &portId)
{
    ProcessorOutputPort* port = getOutput(sourceFilter->id(), portId);
    if(!port)
        return;
    sourceFilter->disconnect(portId, port);
    foreach(ProcessingElement* target, targets())
    {
        target->removePort(port);
    }
    mOutputs.removeAll(port);
    delete port;
}

void Processor::renameOutput(Filter *filter, const QString &portId, const QString &name)
{
    ProcessorOutputPort* port = getOutput(filter->id(), portId);
    if(!port)
        return;
    port->setName(name);
}

bool Processor::hasOutput(const QString &name) const
{
    foreach(ProcessorOutputPort* port, mOutputs)
    {
        if(port->name() == name)
            return true;
    }
    return false;
}

ProcessorOutputPort* Processor::getOutput(int filterId, const QString &portId) const
{
    foreach(ProcessorOutputPort* port, mOutputs)
    {
        if(port->filterId() == filterId && port->portId() == portId)
            return port;
    }
    return 0;
}

QVariantList Processor::getOutputValues()
{
    QMutexLocker lock(&mOutputDataMutex);
    return mDataOut;
}

void Processor::run()
{
    mRunning = true;
    execute();
    mRunning = false;
}

void Processor::execute()
{
    setPriority(mInfo.priority);
    qDebug() << "Processor" << mInfo.name << "started";
    int traceId = Tracer::instance().labelId(name());
    mRunId++;
    Tracer::instance().trace(Tracer::ProcessorStarted, traceId, Tracer::EmptyLabel, mRunId);
    QMutableListIterator<Filter*> filters(mFilters);
    mStop = false;
    mTriggerFinished = false;
    bool stopOnNoOutput = mInfo.stopOnNoOutput;
    try
    {
        while(filters.hasNext())
        {
            Filter* filter = filters.next();
            filter->startup();
            if(mFilterWithErrors.remove(filter))
                emit filterErrorCleared(filters.value()->name(), id());
        }
    }
    catch(const std::exception& err)
    {
        Filter* filter = filters.value();
        while(filters.hasPrevious())
        {
            try
            {
                filters.previous()->shutdown();
            } catch( ... ) {}
        }
        QString errStr = tr("Error starting node %1: %2", "1=name, 2=errorMsg").arg(filter->name(), err.what());
        qCritical() <<  errStr;
        Notifications::newError(errStr);
        emit filterStartupError(filter->name(), err.what(), id());
        return;
    }

    if(mInfo.pausedStartup)
    {
        QMutexLocker lock(&mPauseMutex);
        mPauseOnStart = true;
    }
    qDebug() << "Processor" << mInfo.name << "all filters initialized";
    while(!mStop && !mTriggerFinished)
    {
        int loopId = mLoopId;
        filters.toFront();
        bool working = !stopOnNoOutput; // if not stop, working is true by default
        mPauseMutex.lock();
        if(mPauseOnStart)
            mPauseWait.wait(&mPauseMutex);
        mPauseMutex.unlock();
        if(mHasSource)
        {
            QMutexLocker lock(&mTriggerMutex);
            if(!mInfo.ignoreTrigger)
                mTriggerWait.wait(&mTriggerMutex);
            if(mTriggerFinished || mStop)
                break;
            for(int i=0; i<mTriggerData.size(); i++)
            {
                if(i < mInputs.size())
                    mInputs[i]->notify(mTriggerData[i]);
            }
            loopId = mTriggerId;
        }
        else
        {
            loopId = ++mLoopId;
        }
        try{
            while(filters.hasNext())
            {
                mPauseMutex.lock();
                if(mPaused)
                    mPauseWait.wait(&mPauseMutex);
                mPauseMutex.unlock();
                if(mStop)
                    break;
                Filter* filter = filters.next();
                if(!filter->execute(loopId)) // one filter failed or is finished, skip following filters
                    break;
                if(filter->hasOutputs()) // only filter with outputs have impact to the wokring flag of the processor
                    working = true;
                if(mFilterWithErrors.remove(filter))
                    emit filterErrorCleared(filters.value()->name(), id());
            }
        }
        catch(std::runtime_error& e)
        {
            if(!mFilterWithErrors.contains(filters.value()))
            {
                emit filterExecutionError(filters.value()->name(), e.what(), id());
                QString err = tr("Error executing node %1: %2", "1=name, 2=errorMsg").arg(filters.value()->name(), e.what());
                qWarning() << err;
                Notifications::newWarning(err);
                mFilterWithErrors << filters.value();
            }
        }

        if(!mHasSource && !working)
            mTriggerFinished = true;

        QList<QVariant> dataOut;
        dataOut.reserve(mOutputs.size());
        bool outputsValid = false;

        foreach(ProcessorOutputPort* output, mOutputs)
        {
            outputsValid |= output->updated();
            dataOut.append(output->value());
            output->resetUpdated();
        }
        mOutputDataMutex.lock();
        mDataOut = dataOut;
        foreach(PortListener* listener, mOutputListener)
            listener->setValue(dataOut);
        mOutputDataMutex.unlock();
        if(outputsValid || mOutputs.size() == 0)
        {
            foreach(ProcessingElement* trigger, targets())
            {
                trigger->trigger(dataOut, loopId);
            }
        }
    }
    qDebug() << "Processor" << mInfo.name << "finished";
    if(!mStop) // this was not ended by the user!
        Notifications::newInfo(tr("Image processor %1 has ended").arg(mInfo.name));
    foreach(ProcessingElement* trigger, targets())
    {
        trigger->triggerFinished();
    }
    filters.toFront();
    while(filters.hasNext())
    {
        Filter* filter = filters.next();
        try
        {
            filter->shutdown();
        }
        catch(...){}
    }
    Tracer::instance().trace(Tracer::ProcessorStopped, traceId, Tracer::EmptyLabel, mRunId);
    qDebug() << "Processor" << mInfo.name << "all filters deinitialized";
}

void Processor::stop()
{
    pause();
    mStop = true;
    resume();
    triggerFinished();

}

void Processor::pause()
{
    QMutexLocker lock(&mPauseMutex);
    mPaused = false;
    mPauseOnStart = true;
}

void Processor::resume()
{
    QMutexLocker lock(&mPauseMutex);
    mPaused = false;
    mPauseOnStart = false;
    mPauseWait.wakeAll();
}

void Processor::step(bool singleStep)
{
    QMutexLocker lock(&mPauseMutex);
    mPaused = singleStep;
    mPauseOnStart = !singleStep;
    mPauseWait.wakeAll();
}


bool Processor::hasTarget(ProcessingElement *target) const
{
    return mTargets.contains(target);
}

void Processor::addTarget(ProcessingElement *target)
{
    if(!target->addSource(this))
        return;
    foreach(ProcessorOutputPort* output, outputs())
        target->addPort(output);
    mTargets.insert(target);
}

void Processor::addOutputListener(PortListener *listener)
{
    QMutexLocker lock(&mOutputDataMutex);
    mOutputListener << listener;
}

void Processor::removeOutputListener(PortListener *listener)
{
    QMutexLocker lock(&mOutputDataMutex);
    mOutputListener.remove(listener);
}

void Processor::removeTarget(ProcessingElement *target)
{
    if(!mTargets.contains(target))
        return;
    target->removeSource(this);
    mTargets.remove(target);
}

QList<ProcessingElement*> Processor::targets() const
{
    return mTargets.toList();
}
void Processor::trigger(QList<QVariant> &data, int runId)
{
    QMutexLocker lock(&mTriggerMutex);
    mTriggerData = data;
    mTriggerId = runId;
    mTriggerWait.wakeAll();
}

void Processor::triggerFinished()
{
    {
        QMutexLocker lock(&mTriggerMutex);
        mTriggerFinished = true;
        mTriggerWait.wakeAll();
    }
    resume();
}

bool Processor::addSource(ProcessingElement *)
{
    if(mHasSource)
        return false;
    mHasSource = true;
    return true;
}

void Processor::removeSource(ProcessingElement *)
{
    qDeleteAll(mInputs);
    mInputs.clear();
    mHasSource = false;
}

void Processor::addPort(ProcessorOutputPort *port)
{
    mInputs.append(new ProcessorInputPort(port));
}

void Processor::removePort(ProcessorOutputPort *port)
{
    for(int i=0; i<mInputs.size(); i++)
    {
        if(mInputs[i]->source() == port)
        {
            delete mInputs[i];
            mInputs.removeAt(i);
            break;
        }
    }
}

ProcessorInputPort* Processor::getInputPort(const QString &name) const
{
    foreach(ProcessorInputPort* input, mInputs)
    {
        if(input->source()->name() == name)
            return input;
    }
    return 0;
}
