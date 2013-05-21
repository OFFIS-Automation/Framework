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

#ifndef PROCESSINGELEMENT_H
#define PROCESSINGELEMENT_H

#include <core/ProcessorInfo.h>
#include <core/Filter.h>
class ProcessingElement
{
public:
    ProcessingElement();
    virtual ProcessingElementType type() const = 0;
    virtual int id() const = 0;
    virtual CoreFilter* inputFilter() const;
    virtual CoreFilter* outputFilter() const;

    virtual QList<ProcessingElement*> targets() const;
    virtual ProcessingElement* target(int id) const;
    inline bool hasTarget(int id) const { return target(id) != 0; }

    virtual void addTarget(ProcessingElement* target);
    virtual void removeTarget(ProcessingElement* target);
protected:

    QMap<int, ProcessingElement*> mTargets;

};

#endif // PROCESSINGELEMENT_H
