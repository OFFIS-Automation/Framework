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

#ifndef OLVISCOREINTERFACE_H
#define OLVISCOREINTERFACE_H

#include <core/OlvisInterface.h>
#include <core/FilterInfo.h>
#include <core/ProcessorInfo.h>
#include "CoreGlobal.h"
#include <QMap>
#include <QMutex>
#include <QWaitCondition>
#include <QFileInfo>
#include <QPluginLoader>

class FilterPluginInterface;
class Processor;
class DataJoin;
class DataBuffer;
class Filter;
class MakroFilter;
class FilterGroup;

class OVCORESHARED_EXPORT OlvisCoreInterface : public OlvisInterface
{
    Q_OBJECT
public:
    explicit OlvisCoreInterface();
    virtual ~OlvisCoreInterface();
    virtual QList<FilterTypeInfo> getAllFilterTypes() const;
    virtual FilterTypeInfo getFilterType(const QString& uid) const;
    virtual QList<ProcessorInfo> getProcessors() const;
    virtual ProcessorInfo getProcessor(int id) const;
    virtual ProcessorInfo getProcessor(const QString& name) const;
    virtual QList<FilterInfo> getFilters(int parentId) const;
    virtual QList<FilterInfo> getFilters() const;
    virtual FilterInfo getFilter(int id) const;
    virtual FilterInfo getFilter(const QString& name) const;
    virtual PortInfo getPortInfo(const PortId& portId) const;
    virtual PortInfo getPortInfo(const QString& filterTypeUid, const QString &portId) const;
    virtual QVariant getPortValue(const PortId& port) const;
    virtual bool check() const { QList<PortId> dummy; return check(dummy); }
    virtual bool check(QList<PortId>& unconnectedPorts) const;
    virtual bool isConnected(const PortId& portId) const;
    virtual bool canConnect(const PortId& source, const PortId& target, QString& warning) const;
    virtual bool canConnectInput(const QString& inputName, const PortId& target, QString& warning) const;
    virtual bool canMove(int filterId, int beforeFilter = 0) const;
    virtual QList<JoinInfo> getJoins() const;
    virtual JoinInfo getJoin(int id) const;
    virtual JoinInfo getJoin(const QString& name) const;
    virtual QList<BufferInfo> getDataBuffers() const;
    virtual BufferInfo getDataBuffer(int id) const;
    virtual BufferInfo getDataBuffer(const QString& name) const;
    virtual bool canConnectProcessors(int source, int target, QString& errorMsg) const;
    virtual QList<FilterConnection> getConnections() const;
    virtual QList<FilterConnection> getConnections(int processor) const;
    virtual PortId getSource(const PortId& target) const;
    virtual QList<PortId> getTargets(const PortId& source) const;

    virtual ProcessingElementType processingElementType(int id) const;
    virtual ProcessingElementType processingElementType(const QString& name) const;

    virtual QList<ProcessingElementConnection> processingElementConnections() const;

    QList<QString> processorInputs(int processorId) const;
    QList<PortId> processorInputTargets(int processorId, QString inputName) const;

    virtual int getFilterPosition(int filterId) const;

    virtual QString currentConfig(const QString& targetFile = QString()) const;
    virtual void writeCurrentConfig(QXmlStreamWriter* xml, const QString& targetFile = QString()) const;
    virtual QString portValueString(const PortId& id, const QVariant& var) const;
    virtual QVariant portValueFromString(const PortId& id, const QString& string) const;
    virtual QVariant portValueToSimpleType(const PortId& id, const QVariant& var) const;
    virtual QVariant constrainedValue(const PortId& portId, const QVariant& var) const;
    virtual bool isEmpty() const;

    virtual bool canBeProcessorOutput(const PortId &portId) const;
    virtual QList<FilterGroupPort> processorOutputs(int processorId) const;
    virtual bool isRunning() const { return mIsRunning; }
    static bool hasInstance() { return sInstance != 0; }
    static OlvisCoreInterface* instance() { return sInstance; }

    void setPortValueThrow(const PortId& port, const QVariant& value);

    QVariantList getProcessorOutputValues(int processorId) const;

    QList<FilterGroupInfo> getMakroFilters() const;
    FilterGroupInfo getMakroFilter(int id) const;

    virtual bool clearUpdateFlag();
    virtual bool testUpdateFlag() const;

signals:

public slots:
    virtual void setTracingEnabled(bool enabled);
    virtual int createProcessor(const QString& name);
    virtual void deleteProcessor(int name);
    virtual void renameProcessor(int id, const QString& newName);
    virtual void renameProcessor(const QString& oldName, const QString& newName);
    virtual void setProcessorPriority(int id, int priority);
    virtual void setProcessorPriority(const QString& name, int priority);
    virtual void setProcessorStartupBehavior(int id, bool pausedStartup);
    virtual void setProcessorStartupBehavior(const QString& processorName , bool pausedStartup);

    virtual void setProcessorTriggerBehavior(int id, bool ignoreTrigger);
    virtual void setProcessorTriggerBehavior(const QString& processorName , bool ignoreTrigger);

    virtual int createDataBuffer(int size = 10, const QString& name = QString());
    virtual void resizeDataBuffer(int id, int size);
    virtual void deleteDataBuffer(int id);
    virtual int createJoin(const QString& name = QString());
    virtual void setJoinMode(int id, int mode, int master);
    virtual void deleteJoin(int id);
    virtual void connectProcessor(int source, int target);
    virtual void disconnectProcessor(int source, int target);
    virtual void disconnectProcessor(int id);
    virtual int addFilter(int processor, const QString& filterType, const QString& name = QString(), int insertBefore = -1);
    virtual void deleteFilter(int id);
    virtual void renameFilter(int id, const QString& name);
    virtual void moveFilter(int id, int moveBefore = 0);
    virtual void connectFilter(const PortId& source, const PortId& target);
    virtual void connectProcessorInput(const QString& source, const PortId& target);
    virtual void renameInput(const PortId& target, const QString& newName);
    virtual void disconnectFilter(const PortId& target);
    virtual void disconnectFilters(const PortId& source);
    virtual void setPortValue(const PortId& port, const QVariant& value);
    virtual void createProcessorOutput(const PortId& port, const QString& name = QString());
    virtual void deleteProcessorOutput(const PortId& portId);
    virtual void renameProcessorOutput(const PortId& portId, const QString& newName);

    virtual void scanPluginDir(const QString &pluginDir = QString());

    virtual bool addPortListener(const PortId &portId, PortListener *listener);
    virtual void removePortListener(const PortId &portId, PortListener *listener);

    virtual bool addProcessorListener(int processorId, PortListener *listener);
    virtual void removeProcessorListener(int processorId, PortListener *listener);
    virtual void start();
    virtual void stop();
    virtual bool waitForStop(uint timeout = ULONG_MAX);
    virtual void pause();
    virtual void step(bool singleStep);
    void pauseProcessor(int processor);
    void resumeProcessor(int processor);

    virtual void clear();
    virtual void loadFromData(const QString& str);
    virtual void loadFromFile(const QString& filename);

    virtual int createMakroFilter(const QString& name);
    virtual void deleteMakroFilter(int id);

protected slots:
    void onProcessorFinished();
protected:
    bool mChanged;
    Filter* createFilter(const QString &filterType);
    FilterGroup* getGroup(int id) const;
    QString uniqueProcessorName(QString proposed, const QString& defaultName = "Processor");
    bool canConnectSub(const PortId &sourceId, const PortId &targetId, QString &warning) const;
    int mNextUid;
    QMap<QString, FilterTypeInfo> mFilterTypes;
    QMap<int, Processor*> mProcessors;
    QMap<int, Filter*> mFilters;
    QMap<int, DataJoin*> mJoins;
    QMap<int, DataBuffer*> mBuffers;
    QMap<int, MakroFilter*> mMakroFilters;

    struct CreatorId
    {
        FilterPluginInterface* plugin;
        int position;
        QString name;
    };
    QMap<QString, CreatorId> mCreators;
    QMap<QString, QPluginLoader*> mLoadedPlugins;
    bool mIsRunning;

    static OlvisCoreInterface* sInstance;
    QString mPluginDir;
    int mTraceId;
    QMutex mStopWaitMutex;
    QWaitCondition mStopWait;
};

#endif // OLVISCOREINTERFACE_H
