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

#include "InputPortPrivate.h"
#include <filter/Port.h>
#include <core/PortInfo.h>
#include "PortData.h"

InputPort::InputPort(Port& parent, bool isList) :
    mParent(parent)
{
    parent.portData->in = this;
    parent.setDefaultValueRaw(parent.portData->info.defaultValue);
    parent.portData->info.isArray = isList;
    removeSource();
}

InputPortMode InputPort::mode() const
{
    return mParent.mode();
}

QString InputPort::name() const
{
    return mParent.name();
}

InputPort::~InputPort()
{
}

PortInfo InputPort::getInfo()
{
    return mParent.getInfo();
}

QVariant InputPort::peekValue()
{
    QMutexLocker lock(&mMutex);
    return mValue;
}

void InputPort::setValue(const QVariant &value_)
{
    QVariant value = value_;
    if(value.userType() == QVariant::List)
    {
        const QVariantList& list = value.toList();
        if(list.size() == 0)
            return;
        value = list[0];
    }
    if(value.isValid())
        value = mParent.constrainedValue(value);
    QMutexLocker lock(&mMutex);
    mIsUpdated = true;
    mValue = value;
}

void InputPort::recheck()
{
    QMutexLocker lock(&mMutex);
    if(mValue.isValid())
        mValue = mParent.constrainedValue(mValue);
    emit portValueChanged(name(), mValue);
}

void InputPort::newConstraint()
{
    emit portConstraintsUpdated(name());
}

QVariant InputPort::setDefaultValue(QVariant value)
{
    if(value.isValid())
        value = mParent.constrainedValue(value);
    if(!isConnected())
    {
        setValue(value);
        emit portValueChanged(name(), mValue);
    }
    return value;

}

void InputPort::setSource(QObject *filter, OutputPort *output)
{
    QMutexLocker lock(&mMutex);
    mSourceObject = filter;
    mSourcePort = output;
    if(!isConnected())
        mValue = mParent.getInfo().defaultValue;
}

bool InputPort::hasChanged()
{
    QMutexLocker lock(&mMutex);
    return mLastValue != mValue;
}

void InputPort::unsetChanged()
{
    QMutexLocker lock(&mMutex);
    mIsUpdated = false;
    mLastValue = mValue;
}

InputListPort::InputListPort(Port &parent) : InputPort(parent, true), mIter(mList)
{

}

void InputListPort::setValue(const QVariant &var)
{
    QMutexLocker lock(&mMutex);
    if(var.type() != QVariant::List)
    {
        mList.clear();
        if(var.isValid())
            mList.append(mParent.constrainedValue(var));
    }
    else
    {
        mList = var.toList();
        for(int i=0; i<mList.size(); i++)
            mList[i] = mParent.constrainedValue(mList[i]);
    }
    mIter = mList;
    mIter.toFront();
    mValue = mList;
    mIsUpdated = true;
}

void InputListPort::recheck()
{
    setValue(mList);
    QMutexLocker lock(&mMutex);
    emit portValueChanged(name(), mValue);
}

bool InputListPort::hasValue()
{
    return mIter.hasNext();
}

QVariant InputListPort::getRawValue()
{
    if(!hasValue())
        return QVariant();
    return mIter.next();
}

void InputListPort::unsetChanged()
{
    InputPort::unsetChanged();
    mIter.toFront();
}
