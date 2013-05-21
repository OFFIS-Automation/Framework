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

#ifndef SENSORDATASYSTEM_H
#define SENSORDATASYSTEM_H

#include <QtCore/qglobal.h>
#include <QObject>

#if defined(SENSORDATASYSTEM_LIBRARY)
#  define SENSORDATASYSTEMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SENSORDATASYSTEMSHARED_EXPORT Q_DECL_IMPORT
#endif

class QVariant;


class DataConsumer : public QObject
{
    Q_OBJECT
public:
    virtual ~DataConsumer() {}
    virtual double currentTimestamp() const = 0;
    virtual bool hasProvider() const = 0;
    virtual const QString& identifier() const = 0;
    virtual int numericIdentifier() const = 0;
signals:
    void updated(const QVariant& value, double timestamp, int numId);
    void providerStatusChanged(bool valid, int numId);
};

class DataProvider
{
public:
    virtual void update(const QVariant& var) = 0;
    virtual ~DataProvider() {}
};

class SENSORDATASYSTEMSHARED_EXPORT SensorSystemInterface
{
public:
    static DataProvider* createProvider(const QString& identifier);
    static DataConsumer& consumer(const QString& identifier);
    static bool hasProvider(const QString& identifier);
    static QSet<QString> activeConsumers();
    static QSet<QString> allConsumers();
    static double currentTimestamp();
};


#endif // SENSORDATASYSTEM_H
