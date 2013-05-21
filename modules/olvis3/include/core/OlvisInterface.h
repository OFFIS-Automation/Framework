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

#ifndef OLVISINTERFACE_H
#define OLVISINTERFACE_H

#include <QList>
#include <QObject>
#include <QVariant>
#include <core/PortId.h>
#include <core/FilterInfo.h>
#include <core/ProcessorInfo.h>

enum ProcessingElementType;
struct ProcessingElementConnection;

struct FilterTypeInfo;
struct ProcessorInfo;
struct FilterInfo;
struct BufferInfo;
struct JoinInfo;
struct FilterConnection;
struct FilterGroupPort;

class PortListener;
class QXmlStreamWriter;

class OlvisInterface : public QObject
{
public:
    // READING INTERFACE
    /**
    * returns a list of all filters that can be used. FilterTypeInfo.h must be included, @see FilterTypeInfo for more infos
    */
    virtual QList<FilterTypeInfo> getAllFilterTypes() const = 0;

    /**
    * returns a FilterTypeInfofor the given filter type name
    */
    virtual FilterTypeInfo getFilterType(const QString& uid) const = 0;

    /**
    * returns a list of all configured processors
    */
    virtual QList<ProcessorInfo> getProcessors() const = 0;

    /**
    * returns the info struct of a processor
    */
    virtual ProcessorInfo getProcessor(int id) const = 0;
    virtual ProcessorInfo getProcessor(const QString& name) const = 0;

    /** returns all configured joins */
    virtual QList<JoinInfo> getJoins() const = 0;

    /** returns a specific join */
    virtual JoinInfo getJoin(int id) const = 0;
    virtual JoinInfo getJoin(const QString& name) const = 0;

    /** returns all configured dataBuffers */
    virtual QList<BufferInfo> getDataBuffers() const = 0;

    /** returns a specific data buffer */
    virtual BufferInfo getDataBuffer(int id) const = 0;
    virtual BufferInfo getDataBuffer(const QString& name) const = 0;

    /** return the type of a specific processingElement */
    virtual ProcessingElementType processingElementType(int id) const = 0;
    virtual ProcessingElementType processingElementType(const QString& name) const = 0;

    /** returns all connections of processing elements */
    virtual QList<ProcessingElementConnection> processingElementConnections() const = 0;


    /**
    * returns all filters of a processor
    * the filters are sorted by their order
    */
    virtual QList<FilterInfo> getFilters(int processorId) const = 0;

    /**
    * returns all filters of the system
    */
    virtual QList<FilterInfo> getFilters() const = 0;

    /**
    * returns information struct of a filter id
    */
    virtual FilterInfo getFilter(int id) const = 0;

    /**
    * returns information struct of a filter name
    */
    virtual FilterInfo getFilter(const QString& name) const = 0;


    virtual QList<FilterGroupInfo> getMakroFilters() const = 0;
    virtual FilterGroupInfo getMakroFilter(int id) const = 0;

    /**
    * returns the PortInfo struct of a given filter/port pair
    */
    virtual PortInfo getPortInfo(const PortId& portId) const = 0;
    inline PortInfo getPortInfo(int filterId, const QString& portId) const { return getPortInfo(PortId(filterId, portId)); }
    virtual PortInfo getPortInfo(const QString& filterTypeUid, const QString &portId) const = 0;

    /**
    * for input ports, this returns the current default value for this port
    * for output ports, this returns the last value that was send by the output port
    */
    virtual QVariant getPortValue(const PortId& portId) const = 0;
    inline QVariant getPortValue(int filter, const QString& port) const { return getPortValue(PortId(filter, port)); }

    /**
    * for input ports, this returns whether the input port has a default value set
    * for output ports, this returns whether a last output exists for this port
    */
    inline bool hasPortValue(const PortId& id) const { return hasPortValue(id.filter, id.port); }
    inline bool hasPortValue(int filterId, const QString& portId) const { return getPortValue(filterId, portId).isValid(); }

    /**
    * checks whether a filter ouptup port is configured as processor output port
    */
    virtual bool canBeProcessorOutput(const PortId& portId) const = 0;
    inline bool canBeProcessorOutput(int filterId, const QString& portId) const { return canBeProcessorOutput(PortId(filterId, portId)); }

    /**
    * returns all ids of Filter ports that are outputs of a processor
    */
    virtual QList<FilterGroupPort> processorOutputs(int processorId) const = 0;

    /** checks if the current setup is complete.
    * inserts the unconnected ports the need to be connected or a value assigned into @a unconnectedPorts. the list is cleared by the function
    * @return true if the setup is complete
    */
    virtual bool check(QList<PortId>& unconnectedPorts) const = 0;
    inline bool check() const { QList<PortId> dummy; return check(dummy); }

    /**
    * checks whether a port is connected
    */
    virtual bool isConnected(const PortId& portId) const = 0;
    inline bool isConnected(int nodeId, const QString& portId) const { return isConnected(PortId(nodeId, portId)); }

    /**
    * checks whether a port can be connected
    * if there is a warning for the connection, its written into @a warning
    */
    virtual bool canConnect(const PortId& source, const PortId& target, QString& warning) const = 0;
    virtual bool canConnectInput(const QString& inputName, const PortId& target, QString& warning) const = 0;
    virtual bool canConnect(int sourceFilter, const QString& sourcePort, int targetfilter, const QString& targetPort, QString& warning) const
        { return canConnect(PortId(sourceFilter, sourcePort), PortId(targetfilter, targetPort), warning); }


    /** checks whether a filter can be moved in front of another one
    @a before filter can be <1 if the filter should be moved to the end */
    virtual bool canMove(int filterId, int beforeFilter = 0) const = 0;

    /** returns all connections between filters */
    virtual QList<FilterConnection> getConnections() const = 0;

    /** returns all connections between filters */
    virtual QList<FilterConnection> getConnections(int processor) const = 0;

    /** returns the source connection of a specific target port */
    virtual PortId getSource(const PortId& target) const = 0;
    inline PortId getSource(int filter, const QString& port) const { return getSource(PortId(filter, port)); }

    /** returns all targets of a specific source port */
    virtual QList<PortId> getTargets(const PortId& source) const = 0;
    inline QList<PortId> getTargets(int filter, const QString& port) const { return getTargets(PortId(filter, port)); }

    virtual QString currentConfig(const QString& targetFile = QString()) const = 0;
    virtual void writeCurrentConfig(QXmlStreamWriter* xml, const QString& targetFile = QString()) const = 0;

    virtual QString portValueString(const PortId& id, const QVariant& var) const = 0;
    inline QString portValueString(int filterId, const QString& portId, const QVariant& var) const { return portValueString(PortId(filterId, portId), var); }
    virtual QVariant portValueFromString(const PortId& id, const QString& string) const = 0;
    inline QVariant portValueFromString(int filterId, const QString& portId, const QString& string) const { return portValueFromString(PortId(filterId, portId), string); }
    virtual QVariant constrainedValue(const PortId& portId, const QVariant& var) const = 0;
    inline QVariant constrainedValue(int filterId, const QString& portId, const QVariant& var) const { return constrainedValue(PortId(filterId, portId), var); }


    virtual bool isEmpty() const = 0;
    virtual bool isRunning() const = 0;

    virtual bool clearUpdateFlag() = 0;
    virtual bool testUpdateFlag() const = 0;
    // MODIFYING INTERFACE
public slots:

    /**
    * creates a new processor named @a name
    * @emits processorCreated
    */
    virtual int createProcessor(const QString& name) = 0;

    /**
    * delets the processor named @a name
    * @emits processorDeleted
    */
    virtual void deleteProcessor(int id) = 0;

    /**
    * renames a processor
    * @emits processorRenamed, proceccorUpdated
    */
    virtual void renameProcessor(int id, const QString& newName) = 0;
    virtual void renameProcessor(const QString& oldName, const QString& newName) = 0;

    /**
     * @brief setProcessorPriority
     * @param id the processor id
     * @param priority the new priority
     * @emits processorUpdated
     */
    virtual void setProcessorPriority(int id, int priority) = 0;
    virtual void setProcessorPriority(const QString& name, int priority) = 0;

    /**
     * @brief setProcessorStartupBehavior
     * @param id
     * @param pausedStartup
     * @emits processorUpdated
     */
    virtual void setProcessorStartupBehavior(int id, bool pausedStartup) = 0;
    virtual void setProcessorStartupBehavior(const QString& processorName , bool pausedStartup) = 0;


    /**
    * creates a data buffer to buffer data between processors
    * @emits @see dataBufferCreated
    */
    virtual int createDataBuffer(int size = 10, const QString& name = QString()) = 0;

    /**
    * assings a new size to the buffer identified by @a id
    * @emits @see dataBufferSizeChanged
    */
    virtual void resizeDataBuffer(int id, int newSize) = 0;

    /**
    * removes the data buffer identified by @a id
    * @emits @see dataBufferDeleted
    */
    virtual void deleteDataBuffer(int id) = 0;

    /**
    * creates a join to cleanly concatenate time invariant data of different processors
    * @emits @see joinCreated
    */
    virtual int createJoin(const QString& name = QString()) = 0;

    /**
    * sets the mode of the join identified by @a id.
    * @a joinMode must be of type JoinMode.
    * @a master is only used for JoinMasterMode. Then @a master must be a valid id of a connected input processor
    * @emits @see joinModeChanged
    */
    virtual void setJoinMode(int id, int joinMode, int master) = 0;
    inline void setJoinMode(int id, int joinMode) { setJoinMode(id, joinMode, 0); }

    /**
    * Removes the join identified by @a id
    * @emits @see joinDeleted
    */
    virtual void deleteJoin(int id) = 0;

    /** checks whether processors/joins/buffers may be connected to one another */
    virtual bool canConnectProcessors(int source, int target, QString& errorMsg) const = 0;
    inline bool canConnectProcessors(int source, int target) const{ QString msg; return canConnectProcessors(source, target, msg); }

    /**
    * creates a connection between processors/joins/buffers
    * its not possible to connect a buffer into anything else than a processor, all other combinations are valid
    * @emits @asee processorConnected(int, int) always,
    * @emits one of the other @see processorConnected signals
    */
    virtual void connectProcessor(int source, int target) = 0;

    /**
    * removes a connection between processors/joins/buffers
    * @emits @asee processorDisconnected(int, int) always,
    * @emits one of the other @see processorDisconnected signals
    */
    virtual void disconnectProcessor(int source, int target) = 0;

    /**
    * adds a filter to a processor
    * @emits filterCreated
    */
    virtual int addFilter(int processor, const QString& filterTypeUid, const QString& name = QString(), int insertBefore = 0) = 0;
    virtual int addFilter(int processor, const QString& filterTypeUid, int insertBefore){ return addFilter(processor, filterTypeUid, QString(), insertBefore); }

    /**
    * removes a filter from a processor
    * @emits filterDeleted
    */
    virtual void deleteFilter(int id) = 0;

    /** renames a filter
    * @emits filterRenamed
    */
    virtual void renameFilter(int id, const QString& newName) = 0;

    /** moves a filter to a new position */
    virtual void moveFilter(int id, int moveBefore = 0) = 0;

    /**
    * creates a connection between the ports of the two filters
    * @emits filterConnected
    */
    virtual void connectFilter(const PortId& source, const PortId& target) = 0;
    inline void connectFilter(int sourceFilter, const QString& sourcePort, int targetFilter, const QString& targetPort)
        { connectFilter(PortId(sourceFilter, sourcePort), PortId(targetFilter, targetPort)); }

    virtual void connectProcessorInput(const QString& source, const PortId& target) = 0;
    inline void connectProcessorInput(const QString& source, int targetFilter, const QString& targetPort)
        { connectProcessorInput(source, PortId(targetFilter, targetPort)); }

    virtual void renameInput(const PortId& target, const QString& newName) = 0;
    /** disconnects the connection to the target filter
      * @emits filterDisconnected
    */
    virtual void disconnectFilter(const PortId& target) = 0;
    inline void disconnectFilter(int targetFilter, const QString& targetPort){ disconnectFilter(PortId(targetFilter, targetPort)); }

    /** disconnects all connection from the source filter
      * @emits filterDisconnected multiple times
    */
    virtual void disconnectFilters(const PortId& target) = 0;
    inline void disconnectFilters(int sourceFilter, const QString& sourcePort){ disconnectFilters(PortId(sourceFilter, sourcePort)); }

    /** sets a default value to a port
    */
    virtual void setPortValue(const PortId& portId, const QVariant& value) = 0;
    inline void setPortValue(int filter, const QString& port, const QVariant& value) { setPortValue(PortId(filter, port), value); }


    /**
    * makes an output port of a node an output port of the processor
    */
    virtual void createProcessorOutput(const PortId& portId, const QString& name = QString()) = 0;
    inline void createProcessorOutput(int filterId, const QString& portId, const QString& name = QString())
        { return createProcessorOutput(PortId(filterId, portId), name); }

    virtual void deleteProcessorOutput(const PortId& portId) = 0;
    inline void deleteProcessorOutput(int filterId, const QString& portId)
        { return deleteProcessorOutput(PortId(filterId, portId)); }


    virtual void renameProcessorOutput(const PortId& portId, const QString& newName) = 0;

    virtual void scanPluginDir(const QString& pluginDir = QString()) = 0;

    /** adds a lsitener to a specific output port */
    virtual bool addPortListener(const PortId& portId, PortListener* listener) = 0;
    inline bool addPortListener(int filterId, const QString& portId, PortListener* listener) { return addPortListener(PortId(filterId, portId), listener); }

    /** removes a listener from a specific output port */
    virtual void removePortListener(const PortId& portId, PortListener* listener) = 0;
    inline void removePortListener(int filterId, const QString& portId, PortListener* listener) { return removePortListener(PortId(filterId, portId), listener); }

    /** creates all filters and starts the system */
    virtual void start() = 0;

    /** stops the system execution
     * The method returns immideatly, use the signal executionFinished or the blocking method waitforStop
    */

    virtual void stop() = 0;
    /**
     * waitForStop blocks until all processors are finished
     * @param timeoutInMs maximum wait time
     * @return true if the system was stopped, false if there was a timeout
     */
    virtual bool waitForStop(uint timeoutInMs = ULONG_MAX) = 0;
    /** pauses the execution */
    virtual void pause() = 0;

    virtual void step(bool singleStep) = 0;

    virtual void clear() = 0;
    virtual void loadFromData(const QString& str) = 0;
    virtual void loadFromFile(const QString& filename) = 0;

    virtual int createMakroFilter(const QString& name) = 0;
    virtual void deleteMakroFilter(int id) = 0;

signals:
    /** emmited when a new plugin was loaded or a plugin was reloaded */
    void filterTypesChanged();

    void processorCreated(const ProcessorInfo& info);
    void processorRenamed(const ProcessorInfo& info, const QString& oldName);
    void processorUpdated(const ProcessorInfo& info);
    void processorDeleted(const ProcessorInfo& info);
    void joinCreated(const JoinInfo& info);
    void joinModeChanged(const JoinInfo& info);
    void joinDeleted(const JoinInfo& info);
    void dataBufferCreated(const BufferInfo& info);
    void dataBufferSizeChanged(const BufferInfo& info);
    void dataBufferDeleted(const BufferInfo& info);

    void processorsConnected(int source, int target);
    void processorsDisconnected(int source, int target);
    void processorOutputCreated(const PortId& portId, const QString& name);
    void processorOutputRenamed(const PortId& portId, const QString& name);
    void processorOutputDeleted(const PortId& portId);

    void filterCreated(const FilterInfo& info, int insertBefore);
    void filterDeleted(const FilterInfo& info);
    void filterRenamed(const FilterInfo& info, QString oldName);
    void filterMoved(const FilterInfo& info, int movedBefore);
    void filterOrderChanged(int processorId);
    void filterConnected(const PortId& source, const PortId& target, int processorId);
    void filterDisconnected(const PortId& source, const PortId& target, int processorId);
    void inputPortConnected(const QString& sourceName, const PortId& target, int processorId);
    void inputPortDisconnected(const QString& sourceName, const PortId& target, int processorId);
    void portValueChanged(int filterId, const QString& portId, QVariant value);
    void cleared();
    void executionStarted();
    void executionFinished();
    void filterExecutionError(const QString& filter, const QString& errorMsg, int processorId);
    void filterStartupError(const QString& filter, const QString& errorMsg, int processorId);
    void filterErrorCleared(const QString& filter, int processorId);

    void makroFilterCreated(const FilterInfo& info);
    void makroInputCreated(const QString& name, const PortId& target, int makroId);
    void makroInputRemoved(const PortId& target, int makroId);
    void makroFilterDeleted(int id);

protected:
    OlvisInterface(){ }
private:
    Q_OBJECT
    Q_DISABLE_COPY(OlvisInterface)
};

#endif // OLVISINTERFACE_H
