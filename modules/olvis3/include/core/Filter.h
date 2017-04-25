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

#ifndef FILTER_H
#define FILTER_H

#include <filter/FilterBaseGlobal.h>

#include <QVariant>
#include <QMutex>
#include <QMap>

class UserFilter;
class Port;
class InputPort;
class OutputPort;
struct FilterInfo;
class PortListener;
class PortValueConverter;
class Processor;
class OlvisSensorProvider;

class FILTERBASESHARED_EXPORT Filter : public QObject
{
    Q_OBJECT
public:
    Filter();
    virtual void initialize(int id, const QString& name, int processorId);
    virtual int processorId() const { return mProcessorId; }
    virtual int id() const { return mId; }
    virtual QString name() const { return mName; }
    void setName(const QString& name) { mName = name; }

    virtual QString portValueToString(const QString& portName, const QVariant& value) const;
    virtual QVariant portValueFromString(const QString& portName, const QString& value) const;
    virtual QVariant constrainedPortValue(const QString& portId, const QVariant& value) const;
    virtual QVariant portValueToSimpleType(const QString& portId, const QVariant& value) const;

    virtual void connect(const QString &portId, Filter *targetFilter, const QString &targetPortId);
    virtual void disconnect(const QString& portId, Filter* targetFilter, const QString& targetPort);

    virtual void connect(const QString& portId, PortListener* listener);
    virtual void disconnect(const QString& portId, PortListener* listener);

    virtual bool source(const QString& inputName, QObject*& filter, QString& portName) const;
    virtual bool hasSource(const QString& inputName) const;
    virtual QVariant portValue(const QString& portId);

    virtual FilterInfo info() const = 0;
    virtual bool hasOutputs() const = 0;
    virtual void startup() = 0;
    virtual void shutdown() = 0;
    virtual bool execute(int runId) = 0;

    virtual InputPort* getInput(const QString& name) const = 0;

    virtual void setInput(const QString& port, const QVariant& value) = 0;

    virtual OutputPort* getOutput(const QString& name) const = 0;
    virtual Port* getPort(const QString& name) const = 0;

signals:
    void portValueChanged(int filterId, const QString& portId, const QVariant &value);
    void constraintsUpdated(int filterId, const QString& portId);

protected:
    int mId, mProcessorId;
    QString mName;
    bool mInitialized;
};

#endif // FILTER_H
