// OFFIS Automation Framework
// Copyright (C) 2013-2014 OFFIS e.V.
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

#ifndef MAKROFILTER_H
#define MAKROFILTER_H

#include <core/Filter.h>
#include "FilterGroup.h"
#include <core/ProcessorInfo.h>
class MakroFilter : public Filter, public FilterGroup
{
    Q_OBJECT
public:

    struct Output
    {
        OutputPort* oport;
        Port* port;
        Filter* filter;
        QString realName;
    };

    struct Input{
        InputPort* iport;
        Port* port;
        Filter* filter;
        QString realName;
    };

    MakroFilter(bool instance = false);
    virtual ~MakroFilter();
    virtual void initialize(int id, const QString &name, bool isLocal, int processorId);
    FilterGroupInfo groupInfo() const;
    virtual FilterInfo info() const;
    virtual bool hasOutputs() const;
    virtual void startup();
    virtual void shutdown();
    virtual bool execute(int runId);

    virtual InputPort* getInput(const QString& name) const;

    virtual void setInput(const QString& port, const QVariant& value);

    virtual OutputPort* getOutput(const QString& name) const;
    virtual Port* getPort(const QString& name) const;

    virtual bool hasOutput(int filterId, const QString& portId) const;
    virtual bool hasOutput(const QString& name) const;
    virtual void createOutput(const QString& name, Filter* sourceFilter, const QString& portId);
    void removeOutput(Filter* sourceFilter, const QString& portId);
    void renameOutput(Filter* filter, const QString& portId, const QString& newName);

    void createInput(const QString& name, Filter* targetFilter, const QString& portId);
    void removeInput(int filterId, QString portId);
    bool hasInput(const QString& name) { return mInputs.contains(name); }

    bool hasInput(int filterId, const QString &portId);

    const QMap<QString, Input*>& listOfInputs() { return mInputs; }
    const QMap<QString, Output*>& listOfOutputs() { return mOutputs; }
private slots:
    void onPortValueChanged(int filterId, const QString& portId, const QVariant &value);

protected:
    bool mIsInstance;
    bool mIsLocal;
    QMap<QString, Input*> mInputs;
    QMap<QString, Output*> mOutputs;
    QMutex mPortMutex;
};

#endif // MAKROFILTER_H
