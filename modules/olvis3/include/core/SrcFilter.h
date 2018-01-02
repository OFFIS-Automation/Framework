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

#ifndef SRCFILTER_H
#define SRCFILTER_H

#include "Filter.h"

class SubTracer;

class FILTERBASESHARED_EXPORT SrcFilter : public Filter
{
    Q_OBJECT
public:
    SrcFilter(UserFilter* userFilter);
    void setPluginName(const QString& pluginName) { mPluginName = pluginName; }

    virtual ~SrcFilter();
    virtual void initialize(int id, const QString &name, int processorId);
    virtual void setName(const QString& name);
    QString pluginName() const { return mPluginName; }
    FilterInfo info() const;

    void setInput(const QString& port, const QVariant& value);
    void startup();
    bool hasOutputs() const;
    bool execute(int runId);
    void shutdown();

    InputPort* getInput(const QString& name) const;
    OutputPort* getOutput(const QString& name) const;
    virtual Port* getPort(const QString& name) const;

private slots:
    void onUserPortValueChanged(const QString& portId, const QVariant& value);
    void onUserConstraintChanged(const QString& portId);

private:
    void processBufferedUpdates();
    UserFilter* mUserFilter;
    QString mPluginName;
    QMutex mExecMutex, mUpdateMutex;
    QMap<InputPort*, QVariant> mBufferedPortUpdates;
#ifndef NO_SENSOR_SYSTEM
    QMap<OutputPort*, OlvisSensorProvider*> mDataProvider;
#endif
    SubTracer* mTracer;
};


#endif // SRCFILTER_H
