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

#ifndef CONSUMER_H
#define CONSUMER_H

#include <SensorDataSystem.h>
class SensorSystem;

class Consumer : public DataConsumer
{
public:
    Consumer(SensorSystem &system, const QString& identifier, int numericId);
    virtual double currentTimestamp() const;
    virtual bool hasProvider() const;
    virtual const QString& identifier() const;
    virtual int numericIdentifier() const;
    void update(const QVariant& data);
    void setProvider(bool hasProvider);
protected:
    int mId;
    const SensorSystem& mSystem;
    bool mHasProvider;
    const QString mIdentifier;
};

#endif // CONSUMER_H
