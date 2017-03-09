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

#ifndef DATAJOIN_H
#define DATAJOIN_H

#include <core/ProcessorInfo.h>
#include "ProcessorTriggerInterface.h"

class DataJoin : public ProcessingElement
{
public:
    DataJoin(int id, const QString& name);
    void setMode(JoinInfo::JoinMode mode, int master);
    QString name() { return mInfo.name; }
    JoinInfo info() const { return mInfo; }
    virtual void trigger(QList<QVariant>&, int /*id */ ){}
    virtual void triggerFinished(){}
    virtual int id() const { return mInfo.id; }
    virtual bool addSource(ProcessingElement*) { return true; }
    virtual void removeSource(ProcessingElement* ){}
    virtual void addPort(ProcessorOutputPort*){}
    virtual void removePort(ProcessorOutputPort*){}
    virtual void updatePort(ProcessorOutputPort*){}
    virtual QList<ProcessorOutputPort*> outputs() const { return QList<ProcessorOutputPort*>(); }
protected:
    JoinInfo mInfo;
};

#endif // DATAJOIN_H
