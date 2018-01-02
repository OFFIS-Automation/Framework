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

#ifndef SENSORSYSTEM_H
#define SENSORSYSTEM_H

#include <QObject>
#include <QMutex>
#include <QSet>
#include <QMap>
#include <SensorDataSystem.h>
#include <Timer.h>

class Consumer;

class SensorSystem
{
public:
    static SensorSystem& instance() { return sInstance; }
    virtual ~SensorSystem();
    DataConsumer& consumer(const QString &identifier);
    DataProvider* createProvider(const QString &identifier);
    bool hasProvider(const QString& identifier) const;
    double timestamp() const;
    QSet<QString> consumers(bool activeOnly) const;
private:
    Consumer& getConsumer(const QString& identifier);
    int mNextId;
    static SensorSystem sInstance;
    SensorSystem();
    Q_DISABLE_COPY(SensorSystem)

    QMutex* mMutex;
    QMap<QString, Consumer*> mConsumers;
    Timer mTimer;

};

#endif // SENSORSYSTEM_H
