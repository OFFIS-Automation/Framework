// OFFIS Automation Framework
// Copyright (C) 2013-2016 OFFIS e.V.
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

#ifndef PROCESSORTRIGGERINTERFACE_H
#define PROCESSORTRIGGERINTERFACE_H

#include <QList>

class ProcessorOutputPort;

class ProcessingElement
{
public:
    virtual void trigger(QList<QVariant>& data, int runId) = 0;
    virtual void triggerFinished() = 0;
    virtual int id() const = 0;
    virtual bool addSource(ProcessingElement* source) = 0;
    virtual void removeSource(ProcessingElement* source) = 0;
    virtual QList<ProcessorOutputPort*> outputs() const = 0;
    virtual void addPort(ProcessorOutputPort*) = 0;
    virtual void removePort(ProcessorOutputPort*) = 0;
};

#endif // PROCESSORTRIGGERINTERFACE_H
