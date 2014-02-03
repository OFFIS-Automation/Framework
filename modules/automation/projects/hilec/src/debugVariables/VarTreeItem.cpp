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

#include "VarTreeItem.h"
#include "DictVarItem.h"
#include "ListVarItem.h"
#include "TupleVarItem.h"
#include "InstanceVarItem.h"

#undef slots
#include "Python.h"

quint32 VarTreeItem::sUid = 0;

VarTreeItem::VarTreeItem(const QString &name, _object *obj, VarTreeItem* parent, int index) : mName(name), mParent(parent), mIndex(index)
{
    mUid = sUid++;
    if(obj == 0)
        mItem = 0;
    else if(PyDict_Check(obj))
        mItem = new DictVarItem(parent != 0); // dont quote keys for locals / globals
    else if(PyList_Check(obj))
        mItem = new ListVarItem();
    else if(PyTuple_Check(obj))
        mItem = new TupleVarItem();
    else
        mItem = new InstanceVarItem();
    if(obj != 0)
        mChildMap = mItem->setObject(obj);
}

VarTreeItem::~VarTreeItem()
{
    qDeleteAll(mChildren);
    mChildren.clear();
    mChildMap.clear();
    if(mItem != 0)
        delete mItem;
}


int VarTreeItem::childCount() const
{
    if(mChildMap.empty()) return resolvedChildCount();
    return mChildMap.size();
}

int VarTreeItem::resolvedChildCount() const
{
    return mChildren.size();
}

QList<VarTreeItem*> VarTreeItem::children()
{
    if(mChildren.empty() && !mChildMap.empty())
    {
        int count = 0;
        foreach(const ObjectPair& pair, mChildMap)
        {
            mChildren.append(new VarTreeItem(pair.first, pair.second, this, count++));
        }
    }
    return mChildren;
}

QString VarTreeItem::name() const
{
    return mName;
}

QString VarTreeItem::value() const
{
    if(mItem)
        return mItem->stringValue();
    else return "UNKNOWN";
}

QString VarTreeItem::typeName() const
{
    if(mItem)
        return mItem->typeName();
    else
        return "UNKNOWN";
}

VarTreeItem* VarTreeItem::child(int index)
{
    QList<VarTreeItem*> list = children();
    if(list.size() <= index)
        return 0;
    return list[index];
}

void VarTreeItem::invalidatePython()
{
    mChildMap.clear();
    foreach (VarTreeItem* item, mChildren) {
        item->invalidatePython();
    }
}
