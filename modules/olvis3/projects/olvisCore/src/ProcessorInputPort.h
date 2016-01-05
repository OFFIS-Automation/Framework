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

#ifndef PROCESSORINPUTPORT_H
#define PROCESSORINPUTPORT_H

#include <core/InputPort.h>
#include <QSet>
class ProcessorOutputPort;

class ProcessorInputPort : public QObject
{
    Q_OBJECT
public:
    ProcessorInputPort(ProcessorOutputPort* source);
    virtual ~ProcessorInputPort();
    void addTarget(InputPort* target);
    QSet<InputPort*> targets() { return mTargets; }
    void removeTarget(InputPort* target);
    void notify(const QVariant& var);
    ProcessorOutputPort* source() const { return mSource; }
protected:
    QSet<InputPort*> mTargets;
    ProcessorOutputPort* mSource;
};

#endif // PROCESSORINPUTPORT_H
