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

#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <core/ProcessorInfo.h>
#include "FilterGroup.h"
#include "ProcessorTriggerInterface.h"

class PortListener;
class Filter;
class ProcessorOutputPort;
class ProcessorInputPort;

class Processor : public QThread, public ProcessingElement, public FilterGroup
{
    Q_OBJECT
public:
    Processor(int id, const QString& name, QThread::Priority priority = QThread::NormalPriority);
    virtual ~Processor();
    void stop();
    void pause();
    void resume();
    void step(bool singleStep);
    void setProcessorPriority(Priority priority);
    void setStartupMode(bool pausedStartup);
    void setTriggerMode(bool ignoreTrigger);
    void setStopMode(bool stopOnNoOutputGenerated);

    /* function to avoid race condition between finished() signal and isFinished() of qthread */
    bool isExecutionFinished() const { return !mRunning; }

    ProcessorInfo info() const { return mInfo; }
    virtual int id() const { return mInfo.id; }

    QString name() const { return mInfo.name; }
    void setName(const QString& name);

    void createOutput(const QString& name, Filter* sourceFilter, const QString& portId);
    void removeOutput(Filter* sourceFilter, const QString& portId);
    void renameOutput(Filter* filter, const QString& portId, const QString& newName);
    bool hasOutput(int filterId, const QString& portId) const { return getOutput(filterId, portId) != 0; }
    bool hasOutput(const QString& name) const;

    ProcessorOutputPort* getOutput(int filterId, const QString& portId) const;
    QList<ProcessorOutputPort*> outputs() const { return mOutputs; }
    QList<ProcessorInputPort*> inputs() const { return mInputs; }

    QVariantList getOutputValues();

    bool hasTarget(ProcessingElement* target) const;
    void removeTarget(ProcessingElement* target);
    void addTarget(ProcessingElement* target);
    void addOutputListener(PortListener* listener);
    void removeOutputListener(PortListener* listener);
    QList<ProcessingElement*> targets() const;
    bool addSource(ProcessingElement *source);
    virtual void removeSource(ProcessingElement* source);
    virtual void addPort(ProcessorOutputPort*);
    virtual void removePort(ProcessorOutputPort*);
    ProcessorInputPort* getInputPort(const QString& name) const;
signals:
    void filterStartupError(const QString& filter, const QString& errorMsg, int processorId);
    void filterExecutionError(const QString& filter, const QString& errorMsg, int processorId);
    void filterErrorCleared(const QString& filter, int processorId);
protected:
    void execute();
    virtual void trigger(QList<QVariant> &data, int runId);
    virtual void triggerFinished();
    virtual void run();
    bool mStop, mTriggerFinished, mRunning;
    ProcessorInfo mInfo;

    QSet<PortListener*> mOutputListener;
    QList<QVariant> mTriggerData;
    int mTriggerId;
    QList<ProcessorOutputPort*> mOutputs;
    QList<ProcessorInputPort*> mInputs;
    QMutex mPauseMutex, mTriggerMutex, mOutputDataMutex;
    QVariantList mDataOut;
    QWaitCondition mPauseWait, mTriggerWait;
    bool mPaused, mPauseOnStart;
    QSet<ProcessingElement*> mTargets;
    bool mHasSource;
    int mRunId, mLoopId;
    QSet<Filter*> mFilterWithErrors;
};

#endif // PROCESSOR_H
