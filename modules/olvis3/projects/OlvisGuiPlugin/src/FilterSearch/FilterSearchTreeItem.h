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

#ifndef FILTERSEARCHTREEITEM_H
#define FILTERSEARCHTREEITEM_H

#include <QTreeWidgetItem>
#include <core/FilterTypeInfo.h>

class FilterSearchTreeItem : public QTreeWidgetItem
{
    //Q_OBJECT
public:
    explicit FilterSearchTreeItem(QTreeWidgetItem *parent = 0);

    void setFilterType(const FilterTypeInfo& uid) { mUid = uid; }
    const FilterTypeInfo& getFilterType() { return mUid; }

private:
    FilterTypeInfo mUid;

signals:

public slots:

};

#endif // FILTERSEARCHTREEITEM_H
