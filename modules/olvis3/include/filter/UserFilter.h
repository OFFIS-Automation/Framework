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

#ifndef USERFILTER_H
#define USERFILTER_H

#include <filter/Port.h>
#include "FilterBaseGlobal.h"
#include <QObject>
class Filter;
struct FilterTypeInfo;
struct UserFilterPrivate;

class FILTERBASESHARED_EXPORT UserFilter : public QObject
{
public:
    explicit UserFilter();
    virtual ~UserFilter();
    virtual void initialize() {}
    virtual void start() {}
    virtual void execute() = 0;
    virtual void stop() {}
    virtual void deinitialize() {}

protected:
    void setName(const QString& name);
    void setDesc(const QString& desc);
    void setGroup(const QString& group);
    void addInputPort(Port& port);
    void addInputListPort(Port& port);
    void addOutputPort(Port& port);
    void addOutputListPort(Port& port);
private:
    UserFilterPrivate* d;
    friend class SrcFilter;

};

#endif // USERFILTER_H
