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

#include "OlvisCoreInterface.h"

#include <QSettings>
#include <core/FilterInfo.h>
#include <core/ProcessorInfo.h>
#include <filter/FilterPluginInterface.h>
#include <core/SrcFilter.h>
#include <core/FilterConnection.h>
#include <core/InputPort.h>
#include <core/Tracer.h>
#include <filter/FilterTypeInfoHelper.h>

#include <QDir>
#include <QPluginLoader>
#include <QDebug>

#include "Processor.h"
#include "DataBuffer.h"
#include "DataJoin.h"
#include "ProcessorOutputPort.h"
#include "ProcessorInputPort.h"
#include "ConfigWriter.h"
#include "ConfigReader.h"
#include "MakroFilter.h"

#include <stdexcept>

OlvisCoreInterface* OlvisCoreInterface::sInstance = 0;

OlvisCoreInterface::OlvisCoreInterface()
{
    mIsRunning = false;
    mNextUid = 1;
    sInstance = this;
    Tracer::instance();
    mTraceId = 0;
    mChanged = false;

}

OlvisCoreInterface::~OlvisCoreInterface()
{
    stop();
    waitForStop();
    sInstance = 0;
    qDeleteAll(mProcessors);
    qDeleteAll(mFilters);
    qDeleteAll(mLoadedPlugins);
}

bool OlvisCoreInterface::isEmpty() const
{
    return mProcessors.empty();
}

QList<FilterTypeInfo> OlvisCoreInterface::getAllFilterTypes() const
{
    QList<FilterTypeInfo> list = mFilterTypes.values();
    foreach(MakroFilter* makro, mMakroFilters)
    {
        list.append(makro->info().typeInfo);
    }
    return list;
}

FilterTypeInfo OlvisCoreInterface::getFilterType(const QString &uid) const
{
    foreach(MakroFilter* makro, mMakroFilters)
    {
        FilterTypeInfo typeInfo = makro->info().typeInfo;
        if(typeInfo.uid == uid)
            return typeInfo;
    }
    return mFilterTypes.value(uid, FilterTypeInfo());
}

QList<ProcessorInfo> OlvisCoreInterface::getProcessors() const
{
    QList<ProcessorInfo> list;
    foreach(Processor* proc, mProcessors.values())
        list << proc->info();
    return list;
}

ProcessorInfo OlvisCoreInterface::getProcessor(int id) const
{
    Processor* proc = mProcessors.value(id,0);
    if(proc)
        return proc->info();
    return ProcessorInfo();
}

ProcessorInfo OlvisCoreInterface::getProcessor(const QString& name) const
{
    foreach(Processor* p, mProcessors.values())
    {
        if(p->name() == name)
            return p->info();
    }
    return ProcessorInfo();
}

QList<FilterInfo> OlvisCoreInterface::getFilters(int parentId) const
{
    QList<FilterInfo> list;
    FilterGroup* proc = getGroup(parentId);
    if(!proc)
        return list;
    foreach(Filter* filter, proc->filters())
        list << filter->info();
    return list;
}

QList<FilterInfo> OlvisCoreInterface::getFilters() const
{
    QList<FilterInfo> list;
    foreach(Filter* filter, mFilters.values())
        list << filter->info();
    return list;
}

FilterInfo OlvisCoreInterface::getFilter(int id) const
{
    Filter* filter= mFilters.value(id,0);
    if(filter)
        return filter->info();
    return FilterInfo();
}

FilterInfo OlvisCoreInterface::getFilter(const QString &name) const
{
    QMapIterator<int, Filter*> filters(mFilters);
    while(filters.hasNext())
    {
        FilterInfo info = filters.next().value()->info();
        if(info.name == name)
            return info;
    }
    return FilterInfo();
}

int OlvisCoreInterface::getFilterPosition(int filterId) const
{
    Filter* filter = mFilters.value(filterId, 0);
    if(!filter)
        return -1;
    FilterGroup* proc = getGroup(filter->processorId());
    if(!proc)
        return -1;
    return proc->indexOf(filter);
}

PortInfo OlvisCoreInterface::getPortInfo(const PortId &portId) const
{
    FilterInfo info = getFilter(portId.filter);
    if(!info.isValid())
        return PortInfo();
    PortInfo pInfo = FilterTypeInfoHelper(info.typeInfo).getInputPort(portId.port);
    if(pInfo.isValid())
        return pInfo;
    return FilterTypeInfoHelper(info.typeInfo).getOutputPort(portId.port);
}

PortInfo OlvisCoreInterface::getPortInfo(const QString &filterTypeUid, const QString &portId) const
{
    FilterTypeInfo info = getFilterType(filterTypeUid);
    if(!info.isValid())
        return PortInfo();
    PortInfo pInfo = FilterTypeInfoHelper(info).getInputPort(portId);
    if(pInfo.isValid())
        return pInfo;
    return FilterTypeInfoHelper(info).getOutputPort(portId);
}

QVariant OlvisCoreInterface::getPortValue(const PortId &portId) const
{
    Filter* filter = mFilters.value(portId.filter, 0);
    if(!filter)
        return 0;
    return filter->portValue(portId.port);
}

QList<JoinInfo> OlvisCoreInterface::getJoins() const
{
    QList<JoinInfo> list;
    foreach(DataJoin* join, mJoins.values())
        list << join->info();
    return list;
}

JoinInfo OlvisCoreInterface::getJoin(int id) const
{
    DataJoin* join = mJoins.value(id, 0);
    if(join)
        return join->info();
    else
        return JoinInfo();
}

JoinInfo OlvisCoreInterface::getJoin(const QString &name) const
{
    foreach(DataJoin* join, mJoins.values())
    {
        if(join->name() == name)
            return join->info();
    }
    return JoinInfo();
}

QList<BufferInfo> OlvisCoreInterface::getDataBuffers() const
{
    QList<BufferInfo> list;
    foreach(DataBuffer* buffer, mBuffers.values())
        list << buffer->info();
    return list;
}

BufferInfo OlvisCoreInterface::getDataBuffer(int id) const
{
    DataBuffer* buffer= mBuffers.value(id, 0);
    if(buffer)
        return buffer->info();
    else
        return BufferInfo();
}

BufferInfo OlvisCoreInterface::getDataBuffer(const QString &name) const
{
    foreach(DataBuffer* buffer, mBuffers.values())
    {
        if(buffer->name() == name)
            return buffer->info();
    }
    return BufferInfo();
}

QList<FilterConnection> OlvisCoreInterface::getConnections() const
{
    QList <FilterConnection> values;
    QList<int> keys = mProcessors.keys();
    keys << mMakroFilters.keys();
    foreach(int id, keys)
        values.append(getConnections(id));
    return values;
}

QList<FilterConnection> OlvisCoreInterface::getConnections(int processorId) const
{
    QList <FilterConnection> values;
    FilterGroup* proc = getGroup(processorId);
    if(!proc)
        return values;
    foreach(Filter* filter, proc->filters())
    {
        FilterInfo info = filter->info();
        foreach(PortInfo pInfo, info.typeInfo.inputs)
        {
            QObject* sourceObject;
            QString sourcePort;
            if(filter->source(pInfo.name, sourceObject, sourcePort))
            {
                Filter* source = qobject_cast<Filter*>(sourceObject);
                if(source)
                {
                    int sourceId = mFilters.key(source, 0);
                    int targetId = mFilters.key(filter, 0);
                    FilterConnection conn(PortId(sourceId, sourcePort), PortId(targetId, pInfo.name));
                    values << conn;
                }
            }
        }
    }
    return values;
}

PortId OlvisCoreInterface::getSource(const PortId &target) const
{
    foreach(FilterConnection conn, getConnections())
    {
        if(conn.target == target)
            return conn.source;
    }
    return PortId();
}

QList<PortId> OlvisCoreInterface::getTargets(const PortId &source) const
{
    QList<PortId> targets;
    foreach(FilterConnection conn, getConnections())
    {
        if(conn.source == source)
            targets << conn.target;
    }
    return targets;
}


int OlvisCoreInterface::createProcessor(const QString& nameIn)
{
    if(isRunning()) return 0;
    int id = mNextUid++;
    QString name = uniqueProcessorName(nameIn, "Processor");
    Processor* proc = new Processor(id, name);
    connect(proc, SIGNAL(finished()), SLOT(onProcessorFinished()), Qt::DirectConnection);
    connect(proc, SIGNAL(filterStartupError(QString,QString, int)), SIGNAL(filterStartupError(QString,QString,int)), Qt::QueuedConnection);
    connect(proc, SIGNAL(filterExecutionError(QString,QString, int)), SIGNAL(filterExecutionError(QString,QString,int)), Qt::QueuedConnection);
    connect(proc, SIGNAL(filterErrorCleared(QString,int)), SIGNAL(filterErrorCleared(QString,int)), Qt::QueuedConnection);
    ProcessorInfo info = proc->info();
    mProcessors[id] = proc;
    mChanged = true;
    emit processorCreated(info);
    return info.id;
}

void OlvisCoreInterface::deleteProcessor(int id)
{
    if(isRunning()) return;
    Processor* proc = mProcessors.value(id, 0);
    if(!proc)
        return;

    foreach(Filter* filter, proc->filters())
    {
        deleteFilter(filter->id());
    }
    ProcessorInfo info = proc->info();
    disconnectProcessor(info.id);
    proc->disconnect(this, SLOT(onProcessorFinished()));
    delete proc;
    mProcessors.remove(info.id);
    mChanged = true;
    emit processorDeleted(info);
}

void OlvisCoreInterface::renameProcessor(int id, const QString& newName)
{
    if(isRunning()) return;
    Processor* proc = mProcessors.value(id, 0);
    if(!proc)
        return;
    QString name = uniqueProcessorName(newName, "Processor");
    QString old = proc->name();
    proc->setName(newName);
    mChanged = true;
    emit processorUpdated(proc->info());
    emit processorRenamed(proc->info(), old);
}

void OlvisCoreInterface::renameProcessor(const QString &oldName, const QString &newName)
{
    renameProcessor(getProcessor(oldName).id, newName);
}

void OlvisCoreInterface::setProcessorPriority(int id, int priority)
{
    if(priority < QThread::IdlePriority || priority > QThread::TimeCriticalPriority)
        return;
    Processor* processor = mProcessors.value(id);
    if(!processor)
        return;

    processor->setProcessorPriority(static_cast<QThread::Priority>(priority));
    mChanged = true;
    emit processorUpdated(processor->info());

}

void OlvisCoreInterface::setProcessorPriority(const QString &name, int priority)
{
    setProcessorPriority(getProcessor(name).id, priority);
}

void OlvisCoreInterface::setProcessorStartupBehavior(int id, bool pausedStartup)
{
    Processor* processor = mProcessors.value(id);
    if(!processor)
        return;
    processor->setStartupMode(pausedStartup);
    mChanged = true;
    emit processorUpdated(processor->info());
}

void OlvisCoreInterface::setProcessorStartupBehavior(const QString &processorName, bool pausedStartup)
{
    setProcessorStartupBehavior(getProcessor(processorName).id, pausedStartup);
}

void OlvisCoreInterface::setProcessorTriggerBehavior(int id, bool ignoreTrigger)
{
    Processor* processor = mProcessors.value(id);
    if(!processor)
        return;
    processor->setTriggerMode(ignoreTrigger);
    mChanged = true;
    emit processorUpdated(processor->info());
}

void OlvisCoreInterface::setProcessorTriggerBehavior(const QString &processorName, bool ignoreTrigger)
{
    setProcessorTriggerBehavior(getProcessor(processorName).id, ignoreTrigger);
}

void OlvisCoreInterface::setProcessorStopBehavior(int id, bool stopOnNoOutputGenerated)
{
    Processor* processor = mProcessors.value(id);
    if(!processor)
        return;
    processor->setStopMode(stopOnNoOutputGenerated);
    mChanged = true;
    emit processorUpdated(processor->info());
}

void OlvisCoreInterface::setProcessorStopBehavior(const QString &processorName, bool stopOnNoOutputGenerated)
{
    setProcessorStopBehavior(getProcessor(processorName).id, stopOnNoOutputGenerated);
}

int OlvisCoreInterface::createDataBuffer(int size, const QString &nameIn)
{
    if(isRunning()) return 0;
    int id = mNextUid++;
    QString name = uniqueProcessorName(nameIn, "Buffer");
    DataBuffer* buffer = new DataBuffer(id, name, size);
    mBuffers.insert(id, buffer);
    mChanged = true;
    emit dataBufferCreated(buffer->info());
    return id;
}

void OlvisCoreInterface::resizeDataBuffer(int id, int size)
{
    if(isRunning()) return;
    DataBuffer* buffer = mBuffers.value(id, 0);
    if(!buffer)
        return;
    buffer->setSize(size);
    mChanged = true;
    emit dataBufferSizeChanged(buffer->info());
}

void OlvisCoreInterface::deleteDataBuffer(int id)
{
    if(isRunning()) return;
    DataBuffer* buffer = mBuffers.value(id, 0);
    if(!buffer)
        return;
    BufferInfo info = buffer->info();
    disconnectProcessor(id);
    mBuffers.remove(id);
    delete buffer;
    mChanged = true;
    emit dataBufferDeleted(info);
}

int OlvisCoreInterface::createJoin(const QString &nameIn)
{
    if(isRunning()) return 0;
    int id = mNextUid++;
    QString name = uniqueProcessorName(nameIn, "Join");
    DataJoin* join = new DataJoin(id, name);
    mJoins.insert(id, join);
    mChanged = true;
    emit joinCreated(join->info());
    return id;
}

void OlvisCoreInterface::setJoinMode(int id, int mode, int master)
{
    if(isRunning()) return;
    DataJoin* join = mJoins.value(id, 0);
    if(!join)
        return;
    if(mode < JoinInfo::JoinFirstMode || mode > JoinInfo::JoinLastMode)
        return;
    join->setMode((JoinInfo::JoinMode)mode, master);
    mChanged = true;
    emit joinModeChanged(join->info());
}

void OlvisCoreInterface::deleteJoin(int id)
{
    if(isRunning()) return;
    //@todo
    DataJoin* join = mJoins.value(id, 0);
    if(!join)
        return;
    JoinInfo info = join->info();
    disconnectProcessor(id);
    mJoins.remove(id);
    delete join;
    mChanged = true;
    emit joinDeleted(info);
}

bool OlvisCoreInterface::canConnectProcessors(int source, int target, QString& errorMsg) const
{
    errorMsg = QString();
    if(isRunning())
    {
        errorMsg = tr("Cannot connect processors while core is running");
        return false;
    }
    if(source == target)
    {
        errorMsg = tr("cannot connect an element with itself");
        return false;
    }

    foreach(const ProcessingElementConnection& connection, processingElementConnections())
    {
        if(connection.sourceId == target && connection.targetId == source)
        {
            errorMsg = tr("Cannot connect backwards");
            return false;
        }
    }

    ProcessingElementType sourceType = processingElementType(source);
    ProcessingElementType targetType = processingElementType(target);
    if(sourceType == InvalidElementType || targetType == InvalidElementType)
    {
        errorMsg = tr("One element id is invalid");
        return false;
    }
    if(sourceType == BufferElementType && targetType != ProcessorElementType)
    {
        errorMsg = tr("A buffer can only have a processor as its target");
        return false;
    }

    foreach(ProcessingElementConnection connection, processingElementConnections())
    {
        if(connection.targetId == target && targetType != JoinElementType)
        {
            errorMsg = tr("Multiple sources are not allowed for this target");
            return false;
        }
        if(connection.sourceId == source && sourceType == BufferElementType)
        {
            errorMsg = tr("A buffer is not allowed to have multiple targets");
            return false;
        }
    }
    return true;
}

void OlvisCoreInterface::connectProcessor(int source, int targetId)
{
    if(isRunning()) return;
    QString errorMsg;
    if(!canConnectProcessors(source, targetId, errorMsg))
    {
        qCritical() << errorMsg;
        return;
    }
    Processor* target= mProcessors.value(targetId, 0);
//    if(!target)
//        target = mJoins.value(targetId, 0);
//    if(!target)
//        target = mBuffers.value(targetId, 0);
    if(!target)
        return;
    Processor* sourceProc = mProcessors.value(source, 0);
    if(sourceProc)
        sourceProc->addTarget(target);
    else
        return;
    mChanged = true;
    emit processorsConnected(source, targetId);
    setProcessorStartupBehavior(targetId, target->info().pausedStartup);
}

void OlvisCoreInterface::disconnectProcessor(int id)
{
    if(isRunning()) return;
    foreach(ProcessingElementConnection connection , processingElementConnections())
    {
        if(connection.sourceId == id || connection.targetId == id)
        {
            disconnectProcessor(connection.sourceId, connection.targetId);
            mChanged = true;
        }
    }
}

void OlvisCoreInterface::disconnectProcessor(int source, int targetId)
{
    if(isRunning()) return;
    ProcessingElement* target= mProcessors.value(targetId, 0);
    if(!target)
        target = mJoins.value(targetId, 0);
    if(!target)
        return;
    Processor* sourceProc = mProcessors.value(source, 0);
    if(!sourceProc)
        return;
    if(!sourceProc->hasTarget(target))
        return;
    sourceProc->removeTarget(target);
    mChanged = true;
    emit processorsDisconnected(source, targetId);
}

int OlvisCoreInterface::addFilter(int processor, const QString& filterType, const QString& proposedName, int insertBefore)
{
    if(isRunning())
        return 0;
    FilterGroup* proc = getGroup(processor);
    if(!proc)
        return -1;
    Filter* filter = 0;
    try
    {
        filter = createFilter(filterType);
        if(!filter)
            return -1;
        QString startName = proposedName;
        if(startName.isEmpty())
            startName = filter->info().typeInfo.name;
        QString name = startName;
        for(int nameSuffix = 1; nameSuffix < 999 ;nameSuffix++)
        {
            FilterInfo other = getFilter(name);
            if(!other.isValid())
                break;
            name = startName + QString::number(nameSuffix);
        }

        filter->initialize(mNextUid++, name, processor);

        connect(filter, SIGNAL(portValueChanged(int,QString,QVariant)), SIGNAL(portValueChanged(int,QString,QVariant)));
        mFilters.insert(filter->id(), filter);
        Filter* other = mFilters.value(insertBefore, 0);
        proc->addFilter(filter, other);
        mChanged = true;
        emit filterCreated(filter->info(), insertBefore);
        return filter->id();
    }
    catch(const std::exception& e)
    {
        qCritical() << tr("Could not create filter %1").arg(filterType) << ":" << e.what();
        if(filter)
            delete filter;
        return 0;
    }
}

void OlvisCoreInterface::deleteFilter(int id)
{
    if(isRunning()) return;
    Filter* filter = mFilters.value(id, 0);
    if(!filter)
        return;
    FilterInfo info = filter->info();
    // remove all connections
    foreach(PortInfo pinfo, info.typeInfo.inputs)
    {
        // disconnectfilter will do nothing if the port is not connected
        OlvisInterface::disconnectFilter(info.id, pinfo.name);
    }
    foreach(PortInfo pinfo, info.typeInfo.outputs)
    {
        // disconnectfilters will do nothing if the port is not connected
        OlvisInterface::disconnectFilters(info.id, pinfo.name);
    }
    FilterGroup* group = getGroup(info.processorId);
    if(group)
        group->removeFilter(filter);
    mFilters.remove(info.id);
    delete filter;
    mChanged = true;
    emit filterDeleted(info);
}

void OlvisCoreInterface::renameFilter(int id, const QString& newName)
{
    if(isRunning()) return;
    Filter* filter = mFilters.value(id, 0);
    if(!filter)
        return;
    if(newName.isEmpty())
        return;
    QString name = newName;
    for(int i=1; i<999;i++)
    {
        if(!getFilter(name).isValid())
            break;
        name = newName + QString::number(i);
    }
    QString old = filter->name();
    filter->setName(name);
    mChanged = true;
    emit filterRenamed(filter->info(), old);
}

void OlvisCoreInterface::moveFilter(int id, int moveBefore)
{
    if(isRunning()) return;
    if(!canMove(id, moveBefore))
        return;
    Filter* filter = mFilters.value(id, 0);
    Filter* posFilter  = mFilters.value(moveBefore, 0);
    FilterGroup* proc = getGroup(filter->processorId());
    if(!proc)
        return;
    proc->removeFilter(filter);
    proc->addFilter(filter, posFilter);
    FilterInfo info = filter->info();
    mChanged = true;
    emit filterMoved(info, moveBefore);
    emit filterOrderChanged(info.processorId);
}

bool OlvisCoreInterface::canMove(int filterId, int moveBefore) const
{
    if(isRunning()) return false;
    // check all the connections of this filter to allow/deny the move
    Filter* filter = mFilters.value(filterId, 0);
    if(!filter)
        return false;
    FilterGroup* proc = getGroup(filter->processorId());
    if(!proc)
        return false;
    int targetIndex = proc->indexOf(mFilters.value(moveBefore, 0));
    if(targetIndex < 0)
        targetIndex = proc->filters().size(); // for checks, this can be used

    FilterInfo info = filter->info();
    // get all outputs of this filter
    foreach(PortInfo input, info.typeInfo.inputs)
    {
        PortId source = getSource(PortId(info.id, input.name));
        if(!source.isValid())
            continue;
        int index = proc->indexOf(mFilters.value(source.filter, 0));
        if(index >= targetIndex)
            return false; // connection would be upwards
    }
    foreach(PortInfo output, info.typeInfo.outputs)
    {
        foreach(PortId target, getTargets(PortId(info.id, output.name)))
        {
            int index = proc->indexOf(mFilters.value(target.filter, 0));
            if(index < targetIndex)
                return false; // connection would be upwards
        }
    }
    return true;
}

bool OlvisCoreInterface::canConnect(const PortId &sourceId, const PortId &targetId, QString &warning) const
{
    if(isRunning()) return false;
    if(isConnected(targetId))
        return false;
    FilterInfo source = getFilter(sourceId.filter);
    FilterInfo target = getFilter(targetId.filter);
    if(!source.isValid() || !target.isValid())
        return false;
    if(source.id == target.id)
        return false;
    if(source.processorId != target.processorId)
        return false;
    if(getFilterPosition(source.id) > getFilterPosition(target.id))
        return false;
    return canConnectSub(sourceId, targetId, warning);
}

bool OlvisCoreInterface::canConnectSub(const PortId &sourceId, const PortId &targetId, QString &warning) const
{
    FilterInfo source = getFilter(sourceId.filter);
    FilterInfo target = getFilter(targetId.filter);
    if(!FilterTypeInfoHelper(source.typeInfo).hasOutputPort(sourceId.port))
        return false;
    if(!FilterTypeInfoHelper(target.typeInfo).hasInputPort(targetId.port))
        return false;
    const PortInfo& sourceInfo = FilterTypeInfoHelper(source.typeInfo).getOutputPort(sourceId.port);
    if(!FilterTypeInfoHelper(target.typeInfo).canConnect(targetId.port, sourceInfo, warning))
        return false;

    return true;
}

bool OlvisCoreInterface::canConnectInput(const QString &inputName, const PortId &target, QString &warning) const
{
    Filter* filter = mFilters.value(target.filter, 0);
    if(!filter)
        return false;
    Processor* processor = mProcessors.value(filter->processorId(), 0);
    if(processor)
    {
        ProcessorInputPort* input = processor->getInputPort(inputName);
        if(!input)
            return false;
        ProcessorOutputPort* source = input->source();
        int filterId = source->filterId();
        QString portId = source->portId();
        return canConnectSub(PortId(filterId, portId), target, warning);
    }
    MakroFilter* makro = mMakroFilters.value(filter->processorId(), 0);
    if(makro)
    {
        if(makro->hasInput(target.filter, target.port))
            return false;
        if(!filter->getInput(target.port))
            return false;
        return true;
    }
    return false;
}

void OlvisCoreInterface::connectFilter(const PortId& sourceId, const PortId& targetId)
{
    if(isRunning()) return;
    QString warning;
    if(!canConnect(sourceId, targetId, warning))
        return;
    Filter* source = mFilters.value(sourceId.filter, 0);
    Filter* target = mFilters.value(targetId.filter, 0);
    source->connect(sourceId.port, target, targetId.port);
    mChanged = true;
    emit filterConnected(sourceId, targetId, source->processorId());
}

void OlvisCoreInterface::connectProcessorInput(const QString &sourceName, const PortId &targetId)
{
    if(isRunning()) return;
    QString warning;
    if(!canConnectInput(sourceName, targetId, warning))
        return;
    Filter* filter = mFilters.value(targetId.filter, 0);
    if(!filter)
        return;
    Processor* processor = mProcessors.value(filter->processorId(), 0);
    if(processor)
    {
        ProcessorInputPort* input = processor->getInputPort(sourceName);
        InputPort* target = filter->getInput(targetId.port);
        input->addTarget(target);
        emit inputPortConnected(input->source()->name(), targetId, filter->processorId());
        return;
    }

    MakroFilter* makro = mMakroFilters.value(filter->processorId(), 0);
    if(makro)
    {
        FilterInfo info = filter->info();
        const PortInfo& port = FilterTypeInfoHelper(info.typeInfo).getInputPort(targetId.port);
        QString name = port.name;
        for(int i = 1; i<999; i++)
        {
            if(!makro->hasInput(name))
                break;
            name = port.name + QString::number(i);
        }
        makro->createInput(name, filter, targetId.port);
        mChanged = true;
        emit makroInputCreated(name, targetId, makro->id());
    }

}

void OlvisCoreInterface::renameInput(const PortId &target, const QString &newName)
{
    Filter* filter = mFilters.value(target.filter, 0);
    if(!filter)
        return;
    // currently only allowed for makro filters
    MakroFilter* makro = mMakroFilters.value(filter->processorId());
    if(!makro)
        return;
    if(!makro->hasInput(target.filter, target.port))
        return;
    makro->removeInput(target.filter, target.port);
    emit makroInputRemoved(target, makro->id());

    QString name = newName;
    for(int i = 1; i<999; i++)
    {
        if(!makro->hasInput(name))
            break;
        name = newName + QString::number(i);
    }
    makro->createInput(name, filter, target.port);
    mChanged = true;
    emit makroInputCreated(name, target, makro->id());
}

QList<QString> OlvisCoreInterface::processorInputs(int processorId) const
{
    QList<QString> list;
    Processor* processor = mProcessors.value(processorId, 0);
    if(!processor)
        return list;
    foreach(ProcessorInputPort* input, processor->inputs())
        list << input->source()->name();
    return list;
}

QList<PortId> OlvisCoreInterface::processorInputTargets(int processorId, QString inputName) const
{
    QList<PortId> list;
    Processor* processor = mProcessors.value(processorId, 0);
    if(!processor)
        return list;
     ProcessorInputPort* input = processor->getInputPort(inputName);
     if(!input)
         return list;
    QList<FilterInfo> filters = getFilters(processorId);
    foreach(FilterInfo info, filters)
    {
        Filter* filter = mFilters.value(info.id, 0);
        foreach(PortInfo pInfo, info.typeInfo.inputs)
        {
            InputPort* port = filter->getInput(pInfo.name);
            ProcessorInputPort* source = qobject_cast<ProcessorInputPort*>(port->getSourceFilter());
            if(source && source == input)
                list << PortId(info.id, port->name());
        }
    }
    return list;
}

void OlvisCoreInterface::disconnectFilter(const PortId &target)
{
    if(isRunning()) return;
    Filter* filter = mFilters.value(target.filter, 0);
    if(!filter)
        return;
    QObject* sourceObject;
    QString sourcePort;
    if(!filter->source(target.port, sourceObject, sourcePort))
    {
        // if there is no real source, it may be a makro filter
        MakroFilter* makro = mMakroFilters.value(filter->processorId(), 0);
        if(makro)
        {
            // it is, so this must be an makro filter input
            if(makro->hasInput(target.filter, target.port))
            {
                makro->removeInput(target.filter, target.port);
                mChanged = true;
                emit makroInputRemoved(target, makro->id());
            }
        }
        return;

    }
    Filter* sourceFilter = qobject_cast<Filter*>(sourceObject);
    if(sourceFilter)
    {
        sourceFilter->disconnect(sourcePort, filter, target.port);
        mChanged = true;
        emit filterDisconnected(PortId(sourceFilter->id(), sourcePort), target, filter->processorId());
        return;
    }
    ProcessorInputPort* input = qobject_cast<ProcessorInputPort*>(sourceObject);
    if(input)
    {
        InputPort* targetPort = filter->getInput(target.port);
        input->removeTarget(targetPort);
        mChanged = true;
        emit inputPortDisconnected(input->source()->name(), target, filter->processorId());
    }
}

void OlvisCoreInterface::disconnectFilters(const PortId &source)
{
    if(isRunning()) return;
    Filter* filter = mFilters.value(source.filter, 0);
    if(!filter)
        return;
    foreach(FilterConnection conn, getConnections(filter->processorId()))
    {
        if(conn.source == source)
        {
            Filter* target = mFilters.value(conn.target.filter, 0);
            filter->disconnect(source.port, target, conn.target.port);
            mChanged = true;
            emit filterDisconnected(conn.source, conn.target, filter->processorId());
        }
    }
    deleteProcessorOutput(source);
}

bool OlvisCoreInterface::isConnected(const PortId &portId) const
{
    Filter* filter = mFilters.value(portId.filter, 0);
    if(!filter)
        return false;
    return filter->hasSource(portId.port);

}

void OlvisCoreInterface::setPortValue(const PortId &port, const QVariant &value)
{
    try
    {
        setPortValueThrow(port, value);
    }
    catch(...)
    {
    }
}

void OlvisCoreInterface::setPortValueThrow(const PortId &port, const QVariant &value)
{
    Filter* filter = mFilters.value(port.filter, 0);
    if(!filter)
        throw std::runtime_error(tr("Filter not found").toStdString());
    const FilterInfo& info = filter->info();
    FilterTypeInfoHelper typeInfo(info.typeInfo);
    if(!typeInfo.hasInputPort(port.port))
        throw std::runtime_error(tr("Port not found").toStdString());
    if(typeInfo.portType(port.port) != value.userType())
    {
        // first, try to convert if input is string
        if(value.type() == QVariant::List)
        {
            foreach(QVariant child, value.toList())
            {
                if(typeInfo.portType(port.port) != child.userType())
				{
					QString type1 = QMetaType::typeName(child.userType());
					QString type2 = QMetaType::typeName(typeInfo.portType(port.port));
                    QString errStr = tr("List element is of type %1, type %2 expected").arg(type1, type2);
					throw std::runtime_error(errStr.toStdString());
				}
            }
        }
        else if(!typeInfo.isMode(port.port, OptionalPortMode) || value.isValid()) // dont return if optional ports is set to invalid
        {
            QString type1 = QMetaType::typeName(value.userType());
            QString type2 = QMetaType::typeName(typeInfo.portType(port.port));
            QString errStr = tr("Value is of type %1, type %2 expected").arg(type1, type2);
            throw std::runtime_error(errStr.toStdString());
        }
    }
    filter->setInput(port.port, value);
    mChanged = true;
}

QVariantList OlvisCoreInterface::getProcessorOutputValues(int processorId) const
{
    Processor* p = mProcessors.value(processorId, 0);
    if(p)
        return p->getOutputValues();
    return QVariantList();
}

QList<FilterGroupInfo> OlvisCoreInterface::getMakroFilters() const
{
    QList<FilterGroupInfo> list;
    foreach(MakroFilter* makroFilter, mMakroFilters)
    {
        list << makroFilter->groupInfo();
    }
    return list;
}

FilterGroupInfo OlvisCoreInterface::getMakroFilter(int id) const
{
    MakroFilter* filter = mMakroFilters.value(id, 0);
    if(filter)
        return filter->groupInfo();
    return FilterGroupInfo();
}

bool OlvisCoreInterface::clearUpdateFlag()
{
    bool flag = testUpdateFlag();
    mChanged = false;
    return flag;
}

bool OlvisCoreInterface::testUpdateFlag() const
{
    return mChanged;
}

void OlvisCoreInterface::setTracingEnabled(bool enabled)
{
    Tracer::instance().setEnabled(enabled);
}

bool OlvisCoreInterface::canBeProcessorOutput(const PortId &portId) const
{
    if(isRunning()) return false;
    Filter* filter = mFilters.value(portId.filter, 0);
    if(!filter)
        return false;
    FilterInfo info = filter->info();
    const PortInfo& port = FilterTypeInfoHelper(info.typeInfo).getOutputPort(portId.port);
    if(!port.isValid())
        return false;
    FilterGroup* p = getGroup(filter->processorId());
    if(!p)
        return false;
    if(p->hasOutput(filter->id(), port.name))
        return false;
    return true;
}

QList<FilterGroupPort> OlvisCoreInterface::processorOutputs(int processorId) const
{
    QList<FilterGroupPort> results;
    Processor* p = mProcessors.value(processorId, 0);
    if(p)
    {
        foreach(ProcessorOutputPort* port, p->outputs())
        {
            FilterGroupPort info;
            info.parentId = p->id();
            info.filterId = port->filterId();
            info.name = port->name();
            info.port = getPortInfo(PortId(port->filterId(), port->portId()));
            results << info;
        }
    }
    return results;
}

void OlvisCoreInterface::createProcessorOutput(const PortId &portId, const QString &nameIn)
{
    if(isRunning()) return;
    if(!canBeProcessorOutput(portId))
        return;
    Filter* filter = mFilters.value(portId.filter, 0);
    QString proposedName = nameIn;
    FilterInfo info = filter->info();
    const PortInfo& port = FilterTypeInfoHelper(info.typeInfo).getOutputPort(portId.port);

    if(proposedName.isEmpty())
        proposedName = port.name;
    QString name = proposedName;
    FilterGroup* p = getGroup(filter->processorId());
    if(!p)
        return;
    for(int i = 1; i<999; i++)
    {
        if(!p->hasOutput(name))
            break;
        name = proposedName + QString::number(i);
    }
    p->createOutput(name, filter, portId.port);
    mChanged = true;
    emit processorOutputCreated(portId, name);
}

void OlvisCoreInterface::deleteProcessorOutput(const PortId &portId)
{
    if(isRunning()) return;
    Filter* filter = mFilters.value(portId.filter, 0);
    if(!filter)
        return;
    FilterGroup* processor = getGroup(filter->processorId());
    if(!processor)
        return;
    if(!processor->hasOutput(filter->id(), portId.port))
        return;
    processor->removeOutput(filter, portId.port);
    mChanged = true;
    emit processorOutputDeleted(portId);
}

void OlvisCoreInterface::renameProcessorOutput(const PortId &portId, const QString &newName)
{
    if(isRunning()) return;
    Filter* filter = mFilters.value(portId.filter, 0);
    if(!filter)
        return;
    FilterGroup* processor = getGroup(filter->processorId());
    if(!processor)
        return;
    if(!processor->hasOutput(filter->id(), portId.port))
        return;
    QString name = newName;
    for(int i = 1; i<999; i++)
    {
        if(!processor->hasOutput(name))
            break;
        name = newName + QString::number(i);
    }
    processor->renameOutput(filter, portId.port, name);
    mChanged = true;
    emit processorOutputRenamed(portId, name);
}

bool OlvisCoreInterface::addPortListener(const PortId &portId, PortListener *listener)
{
    Filter* filter = mFilters.value(portId.filter, 0);
    if(!filter)
        return false;
    try
    {
        filter->connect(portId.port, listener);
        return true;
    }
    catch(const std::exception& err)
    {
        qCritical() << err.what();
        return false;
    }
}

void OlvisCoreInterface::removePortListener(const PortId& portId, PortListener *listener)
{
    Filter* filter = mFilters.value(portId.filter, 0);
    if(filter)
        filter->disconnect(portId.port, listener);
}

bool OlvisCoreInterface::addProcessorListener(int processorId, PortListener *listener)
{
    Processor* p = mProcessors.value(processorId, 0);
    if(!p)
        return false;
    p->addOutputListener(listener);
    return true;
}

void OlvisCoreInterface::removeProcessorListener(int processorId, PortListener *listener)
{
    Processor* p = mProcessors.value(processorId, 0);
    if(p)
        p->removeOutputListener(listener);
}

void OlvisCoreInterface::scanPluginDir(const QString &pluginDir_)
{
    if(isRunning()) return;
    QString pluginDir = pluginDir_;
    if(pluginDir.isEmpty())
        pluginDir = mPluginDir;
    if(pluginDir.isEmpty())
        return;
    mPluginDir = pluginDir;
    QSettings settings;
    QStringList skipPlugins =settings.value("olvisCore/disabledPlugins").toStringList();
    mCreators.clear();
    clear();
    // unload all plugins
    foreach(QPluginLoader* loader, mLoadedPlugins.values())
    {
        loader->unload();
        delete loader;
    }
    mLoadedPlugins.clear();
    mFilterTypes.clear();
    QDir dir(pluginDir);
    QStringList suffixFilter = QStringList("*.dll") << "*.so" << "*.dylib";
#ifdef Q_OS_LINUX
    QStringList files = dir.entryList(suffixFilter, QDir::Files | QDir::NoDotAndDotDot, QDir::Name | QDir::IgnoreCase);
#else
    QStringList files = dir.entryList(suffixFilter, QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks, QDir::Name | QDir::IgnoreCase);
#endif


    QListIterator<QString> iter(files);
    qDebug() << "Scanning for plugins:" << dir.canonicalPath();
    dir.mkdir("active");
    QDir workdir = dir;
    workdir.cd("active");
    while(iter.hasNext())
    {
        QString path = iter.next();
        QFileInfo fileInfo(dir.absoluteFilePath(path));

        if(skipPlugins.contains(fileInfo.baseName(), Qt::CaseInsensitive))
            continue;
        QFileInfo target = workdir.absoluteFilePath(path);
        if(target.exists())
            QFile::remove(target.canonicalFilePath());
        QFile::copy(fileInfo.absoluteFilePath(), target.absoluteFilePath());
        QString fullPath = target.absoluteFilePath();
        QPluginLoader* loader = new QPluginLoader(fullPath);
//                loader->setLoadHints(QLibrary::ResolveAllSymbolsHint);
        bool valid = false;
        QObject* object = loader->instance();
        if(object)
        {
            qDebug() << "Found plugin:" << path;
            FilterPluginInterface* plugin = qobject_cast<FilterPluginInterface *>(object);
            if(plugin)
            {
                mLoadedPlugins[fullPath] = loader;
                valid = true;
                qDebug() << "Found plugin with" << plugin->numFilters() << "filters";
                for(int i=0; i<plugin->numFilters(); i++)
                {
                    SrcFilter *filter = plugin->createFilter(i);
                    foreach(QString suffix, suffixFilter)
                    {
                       suffix = suffix.mid(1);
                       if(path.endsWith(suffix))
                       {
                           path.chop(suffix.size());
                           break;
                       }
                    }
                    filter->setPluginName(path);
                    FilterTypeInfo info = filter->info().typeInfo;

                    delete filter;

                    mCreators[info.uid].plugin = plugin;
                    mCreators[info.uid].position = i;
                    mCreators[info.uid].name = info.plugin;
                    mFilterTypes[info.uid] = info;
                    qDebug() << "Found filter:" << info.name << info.uid;
                }
            }
        }
        if(!valid)
        {
            qCritical() << tr("Problem loading olvis plugin %1: %2", "1=file, 2=errorString").arg(path,loader->errorString());
            delete loader;
        }
    }
    emit filterTypesChanged();
}

bool OlvisCoreInterface::check(QList<PortId>& unconnectedPorts) const
{
    bool allOk = true;
    unconnectedPorts.clear();
    foreach(FilterInfo filter, getFilters())
    {
        QListIterator<PortInfo> inputs(filter.typeInfo.inputs);
        while(inputs.hasNext())
        {
            const PortInfo& port = inputs.next();
            PortId portId(filter.id, port.name);
            if(isConnected(portId))
                continue;
            if(hasPortValue(portId))
                continue;
            if(port.mode == OptionalPortMode || port.mode == SingleShotPortMode)
                continue;

            // check if filter is in a makroFilter definition
            MakroFilter* makro = mMakroFilters.value(filter.processorId, 0);
            if(makro)
            {
                if(makro->hasInput(filter.id, port.name))
                    continue;
            }
            else
                allOk = false; // a invalid makro filter does not supress starting
            unconnectedPorts.append(portId);
        }
    }
    return allOk;
}


void OlvisCoreInterface::start()
{
    if(mIsRunning)
    {
        foreach(Processor* p, mProcessors)
            p->resume();
        return;
    }
    if(mFilters.empty())
        return;
    if(!check())
        return;
    qDebug() << tr("Trying to start processor …");
    mTraceId++;
    Tracer::instance().trace(Tracer::SystemStarted, 0, 0, mTraceId);
    foreach(Processor* p, mProcessors)
        p->start();
    qDebug() << tr("Processor started …");
    QMutexLocker lock(&mStopWaitMutex);
    mIsRunning = true;
    emit executionStarted();
}

void OlvisCoreInterface::stop()
{
    if(!mIsRunning)
        return;
    foreach(Processor* p, mProcessors)
        p->stop();
    qDebug() << tr("Stopping image processing …");
}

bool OlvisCoreInterface::waitForStop(uint timeout)
{
    QMutexLocker lock(&mStopWaitMutex);
    if(mIsRunning)
        if(!mStopWait.wait(&mStopWaitMutex, timeout))
            return false;
    return true;
}

void OlvisCoreInterface::onProcessorFinished()
{
    bool allFinished = true;
    foreach(Processor* p, mProcessors)
    {
        if(!p->isExecutionFinished())
            allFinished = false;
    }
    if(!allFinished)
        return;
    Tracer::instance().trace(Tracer::SystemStopped, 0, 0, mTraceId);
    qDebug() << tr("Processor execution has ended");
    QMutexLocker lock(&mStopWaitMutex);
    mIsRunning = false;
    mStopWait.wakeAll();
    emit executionFinished();
}

void OlvisCoreInterface::pause()
{
    foreach(Processor* p, mProcessors)
        p->pause();
    if(!mIsRunning)
        start();
}

void OlvisCoreInterface::step(bool singleStep)
{
    foreach(Processor* p, mProcessors.values())
        p->step(singleStep);
    if(!mIsRunning)
        start();
}

void OlvisCoreInterface::pauseProcessor(int id)
{
    if(!mIsRunning)
        return;
    Processor* p = mProcessors.value(id, 0);
    if(!p)
        return;
    p->pause();
}

void OlvisCoreInterface::resumeProcessor(int id)
{
    if(!mIsRunning)
        return;
    Processor* p = mProcessors.value(id, 0);
    if(!p)
        return;
    p->resume();
}


Filter *OlvisCoreInterface::createFilter(const QString& filterType)
{
    if(!mCreators.contains(filterType))
    {
        // may be a makro filter
        foreach(MakroFilter* makro, mMakroFilters)
        {
            if(makro->info().typeInfo.uid == filterType)
            {
                MakroFilter* newFilter = new MakroFilter(true);
                newFilter->setName(makro->name());
                QMap<QObject*, Filter*> partners;
                foreach(Filter* filter, makro->filters())
                {
                    Filter* subFilter = createFilter(filter->info().typeInfo.uid);
                    newFilter->addFilter(subFilter);
                    partners[filter] = subFilter;
                }
                foreach(Filter* filter, makro->filters())
                {
                    // make the connections
                    foreach(PortInfo info, filter->info().typeInfo.inputs)
                    {
                        partners[filter]->setInput(info.name, filter->portValue(info.name));
                        QObject* source;
                        QString sourcePort;
                        if(filter->source(info.name, source, sourcePort))
                        {
                            Filter* sourceFilter = partners.value(source, 0);
                            if(sourceFilter)
                                sourceFilter->connect(sourcePort, partners[filter], info.name);
                        }
                    }
                }
                QMapIterator<QString, MakroFilter::Input*> inputs(makro->listOfInputs());
                while(inputs.hasNext())
                {
                    inputs.next();
                    MakroFilter::Input* input = inputs.value();
                    newFilter->createInput(inputs.key(), partners[input->filter], input->realName);
                }
                QMapIterator<QString, MakroFilter::Output*> outputs(makro->listOfOutputs());
                while(outputs.hasNext())
                {
                    outputs.next();
                    MakroFilter::Output* output = outputs.value();
                    newFilter->createOutput(outputs.key(), partners[output->filter], output->realName);
                }

                return newFilter;
                break;
            }
        }
    }
    else
    {
        CreatorId creator = mCreators[filterType];
        SrcFilter* srcFilter = creator.plugin->createFilter(creator.position);
        srcFilter->setPluginName(creator.name);
        return srcFilter;
    }
    return 0;
}

FilterGroup *OlvisCoreInterface::getGroup(int id) const
{
    FilterGroup* proc = mProcessors.value(id, 0);
    if(!proc)
        proc = mMakroFilters.value(id, 0);
    return proc;
}

QString OlvisCoreInterface::currentConfig(const QString& targetFile) const
{
    ConfigWriter writer(*this);
    return writer.toXml(targetFile);
}

void OlvisCoreInterface::writeCurrentConfig(QXmlStreamWriter *xml, const QString& targetFile) const
{
    ConfigWriter writer(*this);
    QString filename = targetFile;
    if(targetFile.isEmpty())
    {
        QFile* file = qobject_cast<QFile*>(xml->device());
        if(file)
        {
            QFileInfo info(file->fileName());
            filename = info.absoluteFilePath();
        }
    }
    writer.toXml(xml, filename);
}

QString OlvisCoreInterface::portValueString(const PortId& id, const QVariant &var) const
{
    Filter* filter = mFilters.value(id.filter, 0);
    if(!filter)
        return QString();
    return filter->portValueToString(id.port, var);
}

QVariant OlvisCoreInterface::portValueFromString(const PortId& id, const QString &string) const
{
    Filter* filter = mFilters.value(id.filter, 0);
    if(!filter)
        return QVariant();
    return filter->portValueFromString(id.port, string);
}

QVariant OlvisCoreInterface::portValueToSimpleType(const PortId& id, const QVariant &var) const
{
    Filter* filter = mFilters.value(id.filter, 0);
    if(!filter)
        return QVariant();
    return filter->portValueToSimpleType(id.port, var);
}

QVariant OlvisCoreInterface::constrainedValue(const PortId &portId, const QVariant &var) const
{
    Filter* filter = mFilters.value(portId.filter, 0);
    if(!filter)
        return var;
    return filter->constrainedPortValue(portId.port, var);
}

void OlvisCoreInterface::clear()
{
    if(isRunning()) return;
    foreach(int pid, mProcessors.keys())
    {
        deleteProcessor(pid);
    }
    foreach(int mid, mMakroFilters.keys())
    {
        deleteMakroFilter(mid);
    }
    mChanged = true;
    emit cleared();
}

bool OlvisCoreInterface::loadFromData(const QString &str)
{
    if(isRunning()){
        return false;
    }

    ConfigReader reader(*this, str);
    bool success = reader.createConfig();

    mChanged = true;
    return success;
}

bool OlvisCoreInterface::loadFromFile(const QString &filename)
{
    if(isRunning()){
        return false;
    }

    QFile file(filename);
    file.open(QFile::ReadOnly);

    ConfigReader reader(*this, &file);
    bool success = reader.createConfig();

    mChanged = true;
    return success;
}

int OlvisCoreInterface::createMakroFilter(const QString &proposedName, bool definedInProject)
{
    if(isRunning())
        return -1;
    MakroFilter* filter = new MakroFilter();

    QString startName = proposedName;
    if(startName.isEmpty())
        startName = "MakroFilter";
    QString name = startName;
    for(int nameSuffix = 1; nameSuffix < 999 ;nameSuffix++)
    {
        bool valid = true;
        foreach(Filter* other, mMakroFilters.values())
        {
            if(other->name() == name)
            {
                valid = false;
                break;
            }
        }
        if(valid)
            break;
        name = startName + QString::number(nameSuffix);
    }

    filter->initialize(mNextUid++, name, definedInProject, 0);
    mMakroFilters.insert(filter->id(), filter);
    emit makroFilterCreated(filter->info());
    emit filterTypesChanged();
    mChanged = true;
    return filter->id();
}

void OlvisCoreInterface::deleteMakroFilter(int id)
{
    if(isRunning())
        return;
    MakroFilter* makro = mMakroFilters.value(id, 0);
    if(!makro)
        return;
    foreach(Filter* filter, makro->filters())
    {
        deleteFilter(filter->id());
    }
    delete makro;
    mMakroFilters.remove(id);
    mChanged = true;
    emit makroFilterDeleted(id);
}


QString OlvisCoreInterface::uniqueProcessorName(QString proposed, const QString &defaultName)
{
    if(proposed.isEmpty())
        proposed = defaultName;
    QString name = proposed;
    QStringList names;
    foreach(Processor* p, mProcessors.values())
        names << p->name();
    foreach(DataBuffer* b, mBuffers.values())
        names << b->name();
    foreach(DataJoin* j, mJoins.values())
        names << j->name();
    for(int i = 1; i<999;i++)
    {
        if(!names.contains(name) && !name.isEmpty())
            break;
        name = proposed + QString::number(i);
    }
    return name;
}

ProcessingElementType OlvisCoreInterface::processingElementType(int id) const
{
    if(mProcessors.contains(id))
        return ProcessorElementType;
    else if(mJoins.contains(id))
        return JoinElementType;
    else if(mBuffers.contains(id))
        return BufferElementType;
    else return InvalidElementType;
}

ProcessingElementType OlvisCoreInterface::processingElementType(const QString &name) const
{
    if(getProcessor(name).isValid())
        return ProcessorElementType;
    else if(getJoin(name).isValid())
        return JoinElementType;
    else if(getDataBuffer(name).isValid())
        return BufferElementType;
    else return InvalidElementType;
}

QList<ProcessingElementConnection> OlvisCoreInterface::processingElementConnections() const
{
    QList<ProcessingElementConnection> connections;
    ProcessingElementConnection c;
    c.sourceType = ProcessorElementType;
    foreach(Processor* p, mProcessors.values())
    {
        c.sourceId = p->id();
        foreach(ProcessingElement* target, p->targets())
        {
            c.targetId = target->id();
            c.targetType = processingElementType(c.targetId);
            connections << c;
        }
    }
    return connections;
}
