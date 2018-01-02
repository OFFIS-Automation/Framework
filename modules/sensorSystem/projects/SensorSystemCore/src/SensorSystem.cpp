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

#include "SensorSystem.h"
#include "Consumer.h"
#include "Provider.h"

SensorSystem SensorSystem::sInstance;

SensorSystem::SensorSystem() : mMutex(new QMutex())
{
    mNextId = 1;
    mTimer.start();
}

SensorSystem::~SensorSystem()
{
    delete mMutex;
}

double SensorSystem::timestamp() const
{
    return mTimer.elapsedSeconds();
}

Consumer& SensorSystem::getConsumer(const QString &identifier)
{
    Consumer* consumer = mConsumers.value(identifier, 0);
    if(consumer)
        return *consumer;
    Consumer* newConsumer = new Consumer(*this, identifier, mNextId++);
    mConsumers.insert(identifier, newConsumer);
    return *newConsumer;
}

DataProvider * SensorSystem::createProvider(const QString &identifier)
{
    QMutexLocker lock(mMutex);
    Consumer& c = getConsumer(identifier);
    if(c.hasProvider())
        return 0;
    return new Provider(c);
}



DataConsumer & SensorSystem::consumer(const QString &identifier)
{
    QMutexLocker lock(mMutex);
    return getConsumer(identifier);
}

bool SensorSystem::hasProvider(const QString &identifier) const
{
    QMutexLocker lock(mMutex);
    Consumer* consumer = mConsumers.value(identifier, 0);
    if(consumer)
        return consumer->hasProvider();
    return false;
}

QSet<QString> SensorSystem::consumers(bool activeOnly) const
{
    QMutexLocker lock(mMutex);
    QSet<QString> ids;
    foreach(DataConsumer* consumer, mConsumers.values())
    {
        if(!activeOnly || consumer->hasProvider())
            ids << consumer->identifier();
    }
    return ids;
}

