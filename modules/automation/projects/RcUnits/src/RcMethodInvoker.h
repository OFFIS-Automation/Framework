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

#ifndef RCMETHODINVOKER_H
#define RCMETHODINVOKER_H

#include "RcUnit.h"
#include <QVector>


template <typename T> class RcSingleArgumentWrapper : public RcArgumentWrapper
{
public:
    RcSingleArgumentWrapper(const QByteArray& name)
    {
        mName = name;
    }

    void convert(const QVariant& var)
    {
        mVal = var.value<T>();
    }
    QGenericReturnArgument toArg() {
        return QReturnArgument<T>(mName, mVal);
    }

    QVariant toVariant() { return QVariant::fromValue<T>(mVal); }
protected:
    QByteArray mName;
    T mVal;
};

template <typename T> class RcListArgumentWrapper : public RcArgumentWrapper
{
public:
    RcListArgumentWrapper(const QByteArray& realName)
    {
        mName = realName;
    }
    virtual ~RcListArgumentWrapper()
    {
        mVal.clear();;
    }
    void convert(const QVariant& var)
    {
        foreach(const QVariant& elem, var.toList())
            mVal << elem.value<T>();
    }
    QGenericReturnArgument toArg() {
        return QReturnArgument< QList<T> >(mName, mVal);
    }
    QVariant toVariant()
    {
        QList<QVariant> list;
        foreach(const T& val, mVal)
            list << QVariant::fromValue<T>(val);
        return list;
    }

protected:
    QByteArray mName;
    QList<T> mVal;
};

template <typename T> class RcWrapperFactory : public RcWrapperFactoryItf
{
public:
    RcWrapperFactory(const QByteArray& name) : mName(name) {}
    RcArgumentWrapper* createSingle(){ return new RcSingleArgumentWrapper<T>(mName); }
    RcArgumentWrapper* createList(const QByteArray& realName){ return new RcListArgumentWrapper<T>(realName); }
    QList<int> getListDef() { return QList<int>(); }
protected:
    QByteArray mName;
};

class RcMethodInvoker : public QObject
{
public:
    RcMethodInvoker(const RcUnit::Method& method, QMap<int, RcWrapperFactoryItf*>& factories);
    virtual ~RcMethodInvoker();
    bool parseArguments(const QList<QVariant>& arguments, QString &error);
    void parseArgument(const QVariant& var, int pos);
    void execute(QObject* object);
    QVariant returnValue();
    void checkType(const QVariant& arg, int specType, int pos);
protected:
    QList<RcArgumentWrapper*> mWrapper;
    RcArgumentWrapper* mReturnWrapper;
    const RcUnit::Method& mMethod;
    RcArgumentWrapper* createWrapper(bool list, int type, const QByteArray& listTypeName = QByteArray());
    QMap<int, RcWrapperFactoryItf*>& mFactories;
};

#endif // RCMETHODINVOKER_H
