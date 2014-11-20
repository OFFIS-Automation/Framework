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

#include <SensorDataSystem.h>

#include "SensorSystem.h"

DataConsumer& SensorSystemInterface::consumer(const QString &identifier)
{
    return SensorSystem::instance().consumer(identifier);
}

DataProvider* SensorSystemInterface::createProvider(const QString& identifier)
{
    return SensorSystem::instance().createProvider(identifier);
}

bool SensorSystemInterface::hasProvider(const QString &identifier)
{
    return SensorSystem::instance().hasProvider(identifier);
}

double SensorSystemInterface::currentTimestamp()
{
    return SensorSystem::instance().timestamp();
}

QSet<QString> SensorSystemInterface::activeConsumers()
{
    return SensorSystem::instance().consumers(true);
}

QSet<QString> SensorSystemInterface::allConsumers()
{
    return SensorSystem::instance().consumers(false);
}
