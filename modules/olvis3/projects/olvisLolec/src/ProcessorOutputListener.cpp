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

#include "ProcessorOutputListener.h"

ProcessorOutputListener::ProcessorOutputListener(int processorId)
{
    mSkip = 0;
    mProcessorId = processorId;
    mInfos = OlvisCoreInterface::instance()->processorOutputs(processorId);
}

void ProcessorOutputListener::setValue(const QVariant &var)
{
    QMutexLocker lock(&mMutex);
    mUpdateValue = var;
    if(--mSkip < 0)
        mWait.wakeAll();
}

bool ProcessorOutputListener::wait(unsigned long timeoutMs, int skip)
{
    bool success = false;
    {
        QMutexLocker lock(&mMutex);
        mSkip = skip;
        OlvisCoreInterface::instance()->addProcessorListener(mProcessorId, this);
        success = mWait.wait(&mMutex, timeoutMs);
        mFixedValue = mUpdateValue;
    }
    // the following line must be outside the lock scope, to avoid deadlock
    OlvisCoreInterface::instance()->removeProcessorListener(mProcessorId, this);
    return success;
}

QVariantMap ProcessorOutputListener::values()
{
    QVariantMap data;
    QList<QVariant> dataList;
    {
        QMutexLocker lock(&mMutex);
        if(!mFixedValue.isValid())
            mFixedValue = OlvisCoreInterface::instance()->getProcessorOutputValues(mProcessorId);
        if(mFixedValue.type() != QVariant::List)
            return data;
        dataList = mFixedValue.toList();
    }
    for(int i=0; i<mInfos.size(); i++)
    {
        const FilterGroupPort& info = mInfos[i];
        PortId id(info.filterId, info.port.name);
        QVariant var = dataList.value(i);
        if(var.type() == QVariant::List)
        {
            QList<QVariant> newList;
            foreach(const QVariant& elem, var.toList())
                newList << OlvisCoreInterface::instance()->portValueToSimpleType(id, elem);
            var = newList;
        }
        else if(var.isValid())
            var = OlvisCoreInterface::instance()->portValueToSimpleType(id, var);
        data[info.name] = var;
    }
    return data;
}

