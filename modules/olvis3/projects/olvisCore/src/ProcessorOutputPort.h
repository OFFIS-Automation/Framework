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

#ifndef PROCESSOROUTPUTPORT_H
#define PROCESSOROUTPUTPORT_H

#include <core/PortListener.h>
#include <core/ProcessorInfo.h>

class ProcessorOutputPort : public PortListener
{
public:
    ProcessorOutputPort(const QString& name, int filterId, const QString& portId);
    virtual void setValue(const QVariant& v);
    int filterId() const { return mFilterId; }
    QString name() const { return mName; }
    QString portId() const { return mPortId; }
    bool updated() { return mUpdated; }
    void resetUpdated() { mUpdated = false; }
    QVariant& value() { return mValue; }
    void setName(const QString& name) { mName = name; }
protected:
    QString mName, mPortId;
    int mFilterId;
    QVariant mValue;
    bool mUpdated;

};

#endif // PROCESSOROUTPUTPORT_H
