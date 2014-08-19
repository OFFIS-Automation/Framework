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

#include "VariablesModel.h"

#include "VarTreeItem.h"
#include <QColor>

#undef slots
#include "Python.h"

VariablesModel::VariablesModel(QObject *parent) :
    QAbstractItemModel(parent), mMutex(new QMutex(QMutex::Recursive)), mItems(new QMap<quint32, VarTreeItem*>())
{
    mLocals = new VarTreeItem("locals", 0);
}

VariablesModel::~VariablesModel()
{
    delete mMutex;
    delete mLocals;
    delete mItems;
}

QVariant VariablesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();
    if(section == 0)
        return QString("Name");
    else if(section == 1)
        return QString("Value");
    else if(section == 2)
        return QString("Type");
    else
        return QVariant();
}

QModelIndex VariablesModel::index(int row, int column, const QModelIndex &parent) const
{
    QModelIndex index = QModelIndex();
    QMutexLocker lock(mMutex);
    VarTreeItem* item, *childItem = 0;
    if(!parent.isValid())
        item = mLocals;
    else
        item = mItems->value(parent.internalId(), 0);
    if(item)
        childItem = item->child(row);
    if(childItem)
    {
        index = createIndex(row, column, childItem->uid());
        if(!mItems->contains(childItem->uid()))
            mItems->insert(childItem->uid(),childItem);
    }
    return index;
}

QModelIndex VariablesModel::parent(const QModelIndex &child) const
{
    QModelIndex index = QModelIndex();
    QMutexLocker lock(mMutex);
    if(child.isValid())
    {
        VarTreeItem* item = mItems->value(child.internalId());
        if(item)
        {
            VarTreeItem* parent = item->parent();
            if(parent)
            {
                index = createIndex(parent->index(), 0, parent->uid());
                if(!mItems->contains(parent->uid()))
                    mItems->insert(parent->uid(),parent);
            }
        }

    }
    return index;
}

int VariablesModel::rowCount(const QModelIndex &parent) const
{
    QMutexLocker lock(mMutex);
    int rows = 0;
    VarTreeItem *parentItem;
    if (parent.column() <= 0)
    {
        if (!parent.isValid())
            parentItem = mLocals;
        else
            parentItem = mItems->value(parent.internalId(), 0);
        if(parentItem)
            rows = parentItem->childCount();
    }
    return rows;
}

int VariablesModel::columnCount(const QModelIndex &) const
{
    return 3;
}

bool VariablesModel::hasChildren(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return false;
    QMutexLocker lock(mMutex);
    VarTreeItem *parentItem;
    if(!parent.isValid())
        parentItem = mLocals;
    else
        parentItem = mItems->value(parent.internalId(), 0);
    if(!parentItem)
        return false;
    return parentItem->childCount() > 0;
}

void VariablesModel::setVariables(_object *vars)
{
    beginResetModel();
    lock();
    mMutex->lock();
    mItems->clear();
    delete mLocals;
    mLocals = new VarTreeItem("locals", vars, 0, 0);
    mMutex->unlock();
    unlock();
    endResetModel();
}

void VariablesModel::lock()
{
    emit lockChanged(true);
}

void VariablesModel::unlock()
{
    emit lockChanged(false);
}

void VariablesModel::invalidatePython()
{
    return setVariables(0);
    beginResetModel();
    lock();
    mMutex->lock();
    mItems->clear();
    mLocals->invalidatePython();
    mMutex->unlock();
    unlock();
    endResetModel();
}

QVariant VariablesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    QMutexLocker lock(mMutex);
    VarTreeItem *item = mItems->value(index.internalId(), 0);
    if(!item)
        return QVariant();



    //if(role == Qt::TextColorRole)
    //    return red if updated, QVariant() else

    if (role != Qt::DisplayRole && role != Qt::ToolTipRole)
        return QVariant();
    QVariant value;
    if(role == Qt::ToolTipRole)
        value = item->value();
    else if(index.column() == 1)
        value = item->value().simplified();
    else if(index.column() == 0)
        value = item->name();
    else if(index.column() == 2)
        value = item->typeName();
    return value;
}
