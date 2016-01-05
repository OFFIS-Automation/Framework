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

#ifndef VARIABLESMODEL_H
#define VARIABLESMODEL_H

#include <QAbstractItemModel>
#include <QMutex>

struct _object;
class VarTreeItem;

class VariablesModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit VariablesModel(QObject *parent = 0);
    virtual ~VariablesModel();
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const;
    void setVariables(_object* vars);
    void lock();
    void unlock();
    void invalidatePython();
signals:
    void lockChanged(bool locked);
public slots:

protected:
    QMutex* mMutex;
    VarTreeItem* mLocals;
    QMap<quint32, VarTreeItem*>* mItems;
};

#endif // VARIABLESMODEL_H
