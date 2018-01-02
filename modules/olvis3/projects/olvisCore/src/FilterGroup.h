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

#ifndef FILTERGROUP_H
#define FILTERGROUP_H

#include <QList>
class Filter;
class FilterGroup {
public:
    virtual QList<Filter*> filters() const { return mFilters; }
    virtual int indexOf(Filter* filter) const { return mFilters.indexOf(filter); }
    virtual void addFilter(Filter* filter, Filter* targetPosition = 0);
    virtual void removeFilter(Filter *filter);
    virtual bool hasOutput(int filterId, const QString& portId) const = 0;
    virtual bool hasOutput(const QString& name) const = 0;
    virtual void createOutput(const QString& name, Filter* sourceFilter, const QString& portId) = 0;
    virtual void removeOutput(Filter* filter, const QString& portId) = 0;
    virtual void renameOutput(Filter* filter, const QString& portId, const QString& newName) = 0;
protected:
    QList<Filter*> mFilters;
};

#endif // FILTERGROUP_H
