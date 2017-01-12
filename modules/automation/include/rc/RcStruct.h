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

#ifndef RCSTRUCT_H
#define RCSTRUCT_H

#include <QStringList>
#include <QDebug>
#include "RcUnitInterface.h"

class RcArgumentWrapper
{
public:
    virtual ~RcArgumentWrapper() {}
    virtual void convert(const QVariant& var) = 0;
    virtual QGenericReturnArgument toArg() = 0;
    virtual QVariant toVariant() = 0;
};

template <typename T> class RcStructArgumentWrapper : public RcArgumentWrapper
{
public:
    RcStructArgumentWrapper(const QByteArray& name) : mName(name) {}
    virtual void convert(const QVariant& var){ mVal << var.toList(); }
    virtual QGenericReturnArgument toArg(){ return QReturnArgument<T>(mName, mVal); }
    virtual QVariant toVariant(){ QList<QVariant> var; var << mVal; return var; }
protected:
    QByteArray mName;
    T mVal;
};

template <typename T> class RcStructListArgumentWrapper : public RcArgumentWrapper
{
public:
    RcStructListArgumentWrapper(const QByteArray& name) : mName( "QList<" + name + ">") {}
    virtual void convert(const QVariant& list)
    {
        foreach(const QVariant& var, list.toList())
        {
            T val;
            val << var.toList();
            mVal << val;
        }
    }
    virtual QGenericReturnArgument toArg(){ return QReturnArgument< QList<T> >(mName, mVal); }
    virtual QVariant toVariant(){
        QList<QVariant> list;
        foreach(const T& elem, mVal)
        {
            QList<QVariant> val;
            val << elem;
            list << QVariant(val);
        }
        return list;
    }
protected:
    QByteArray mName;
    QList<T> mVal;
};

class RcWrapperFactoryItf
{
public:

    virtual RcArgumentWrapper* createSingle() = 0;
    virtual RcArgumentWrapper* createList(const QByteArray& realName) = 0;
    virtual QList<int> getListDef() = 0;
};

template <typename T> class RcWrapperStructFactory : public RcWrapperFactoryItf
{
public:
    RcWrapperStructFactory(const QByteArray& name, const QList<int> def) : mName(name), mDef(def) {}
    RcArgumentWrapper* createSingle() { return new RcStructArgumentWrapper<T>(mName); }
    RcArgumentWrapper* createList(const QByteArray& listName) { return new RcStructListArgumentWrapper<T>(listName); }
    QList<int> getListDef() { return mDef; }
protected:
    QByteArray mName;
    QList<int> mDef;
};

template <typename T> void rcRegisterStruct(const QByteArray& name, const QStringList& names, RcBase& rc)
{
    int id = qRegisterMetaType<T>(name);
    // check if T can be converted into QVariant
    T t;
    QList<QVariant> l;
    l << t;
    t << l;
    // get types
    QList<int> types;
    foreach(const QVariant& v, l)
        types << v.userType();
    if(types.length() != names.length())
    {
        qCritical() << "rcRegisterStruct: given name array length does not match number of elements";
        return;
    }

    rc.registerStruct(id, name, names, types, new RcWrapperStructFactory<T>(name, types));
}


template <typename T> void rcRegisterStruct(const QByteArray& name, const QString& names, RcBase& rc)
{
    rcRegisterStruct<T>(name, names.split(",", QString::SkipEmptyParts), rc);
}



#endif // RCSTRUCT_H
