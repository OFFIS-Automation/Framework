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

#ifndef VARTREEITEM_H
#define VARTREEITEM_H

#include <QString>
#include <QMap>
#include <QPair>

struct _object;
class AbstractVarItem;

class VarTreeItem
{
public:
    VarTreeItem(const QString& name, _object* obj, VarTreeItem* parent = 0, int index = 0);
    virtual ~VarTreeItem();
    QString name() const;
    QString value() const;
    QString typeName() const;
    VarTreeItem* parent() const { return mParent; }
    int childCount() const;
    int resolvedChildCount() const;
    int index() const { return mIndex; }
    QList<VarTreeItem*> children();
    VarTreeItem* child(int index);
    int uid() const { return mUid; }
    void invalidatePython();
protected:
    quint32 mUid;
    static quint32 sUid;
    int mIndex;
    QString mName;
    VarTreeItem* mParent;
    AbstractVarItem* mItem;
    typedef QPair<QString, _object*> ObjectPair;
    typedef QList<ObjectPair> ObjectList;
    ObjectList mChildMap;
    QList<VarTreeItem*> mChildren;
};

#endif // VARTREEITEM_H
