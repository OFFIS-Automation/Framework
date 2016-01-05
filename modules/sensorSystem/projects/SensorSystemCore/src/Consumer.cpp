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

#include "Consumer.h"
#include "SensorSystem.h"
#include <QDebug>

Consumer::Consumer(SensorSystem &system, const QString &identifier, int numericId)
    : mId(numericId),
      mSystem(system),
      mHasProvider(false),
      mIdentifier(identifier)
{
    qDebug() << "DataConsumer created" << identifier << mId;
}

double Consumer::currentTimestamp() const
{
    return mSystem.timestamp();
}

bool Consumer::hasProvider() const
{
    return mHasProvider;
}

const QString &Consumer::identifier() const
{
    return mIdentifier;
}

int Consumer::numericIdentifier() const
{
    return mId;
}

void Consumer::update(const QVariant &data)
{
    emit updated(data, currentTimestamp(), mId);
}

void Consumer::setProvider(bool hasProvider)
{
    mHasProvider = hasProvider;
    emit providerStatusChanged(hasProvider, mId);
}




