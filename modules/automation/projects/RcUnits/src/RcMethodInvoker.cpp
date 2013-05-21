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

#include "RcMethodInvoker.h"
#include <lolecs/RcExceptions.h>

RcMethodInvoker::RcMethodInvoker(const RcUnit::Method &method, QMap<int, RcWrapperFactoryItf *> &factories)
    : mMethod(method),
      mFactories(factories)
{
    mReturnWrapper = 0;
}

RcMethodInvoker::~RcMethodInvoker()
{
    qDeleteAll(mWrapper);
    if(mReturnWrapper)
        delete mReturnWrapper;
}

void RcMethodInvoker::parseArguments(const QList<QVariant> &arguments)
{
    if(mMethod.arguments.count() != arguments.count())
        throw RcError(tr("Wrong parameter count, expected %1, got %2").arg(mMethod.arguments.count()).arg(arguments.count()));
    for(int i=0; i<arguments.size(); i++)
        parseArgument(arguments[i], i);
    if(mMethod.hasReturn)
        mReturnWrapper = createWrapper(mMethod.returnType.type == RcUnit::Parameter::List, mMethod.returnType.typeId, mMethod.returnType.realName);
}

void RcMethodInvoker::parseArgument(const QVariant &var, int pos)
{
    RcArgumentWrapper* wrapper;
    const RcUnit::Parameter& spec = mMethod.arguments[pos];
    if(spec.type == RcUnit::Parameter::List)
    {
        if(var.type() != QVariant::List && var.type() != QVariant::StringList) // check type
            throw RcError(tr("Wrong argument type at position %1: list expected").arg(pos));
        QList<QVariant> list = var.toList();
        if(list.size() < spec.min)
            throw RcError(tr("List is to short at position %1: min: %2, elements: %3").arg(pos).arg(spec.min).arg(list.size()));
        if(spec.max > 0 && spec.max < list.size())
            throw RcError(tr("List is to long at position %1: max: %2, elements: %3").arg(pos).arg(spec.max).arg(list.size()));
        // check all inner types and create list
        foreach(const QVariant& elem, list)
        {
            checkType(elem, spec.typeId, pos);
        }
        wrapper = createWrapper(true, spec.typeId, spec.realName);
    }
    else // plain type
    {
        checkType(var, spec.typeId, pos);
        wrapper = createWrapper(false, spec.typeId);
    }
    if(!wrapper)
        throw RcError(tr("Could not create wrapper for argument type %1 at position &2").arg(QString(spec.name)).arg(pos));
    wrapper->convert(var);
    mWrapper << wrapper;
}

void RcMethodInvoker::execute(QObject *object)
{
    QVector<QGenericArgument> args(10);
    int pos = 0;
    foreach(RcArgumentWrapper* wrapper, mWrapper)
        args[pos++] = wrapper->toArg();
    if(mReturnWrapper)
        mMethod.meta.invoke(object, Qt::DirectConnection, mReturnWrapper->toArg(), args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]);
    else
        mMethod.meta.invoke(object, Qt::DirectConnection, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]);
}

QVariant RcMethodInvoker::returnValue()
{
    if(mReturnWrapper)
        return mReturnWrapper->toVariant();
    return QVariant();
}

void RcMethodInvoker::checkType(const QVariant& arg, int specType, int pos)
{
    if(specType == QMetaType::QVariant)
        return;
    if(arg.type() == QVariant::Double && specType == QMetaType::Float)
        return;
    if(arg.type() == QVariant::List)
    {
        if(!mFactories.contains(specType))
            throw RcError(tr("Internal error at position %1: could not read custom struct specification").arg(pos));
        QList<QVariant> data = arg.toList();
        QList<int> types = mFactories[specType]->getListDef();
        if(types.size() != data.size())
            throw RcError(tr("Wrong custom argument lenght at position %1: expected %2, got %3").arg(pos).arg(types.size()).arg(data.size()));
        for(int i=0; i<types.size(); i++)
        {
            checkType(data[i], types[i], pos);
        }
        return;
    }
    if(arg.userType() != specType)
        throw RcError(tr("Wrong argument type at position %1: expected %3, got %2").arg(pos).arg(QMetaType::typeName(arg.userType())).arg(QMetaType::typeName(specType)));
}

RcArgumentWrapper* RcMethodInvoker::createWrapper(bool list, int type, const QByteArray& listTypeName)
{
    if(mFactories.contains(type))
    {
        if(list)
            return mFactories[type]->createList(listTypeName);
        else
            return mFactories[type]->createSingle();
    }

    return 0;
}
