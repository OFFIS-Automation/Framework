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

#include "SensorSystemLolec.h"
#include <SensorDataSystem.h>

#include <QSet>
SensorSystemLolec::SensorSystemLolec() :
    mWriter(SensorDataWriter::instance())
{
}

QList<QString> SensorSystemLolec::getAllSensors()
{
    return SensorSystemInterface::allConsumers().toList();
}

QList<QString> SensorSystemLolec::getActiveSensors()
{
    return SensorSystemInterface::activeConsumers().toList();
}

void SensorSystemLolec::addToTrace(const QString &sensorName)
{
    mWriter.add(sensorName);
}

void SensorSystemLolec::removeFromTrace(const QString &sensorName)
{
    mWriter.remove(sensorName);
}

QList<QString> SensorSystemLolec::getTracedSensors()
{
    return mWriter.tracedItems();
}

QString SensorSystemLolec::addMarker(const QString &markerName)
{
    return mWriter.mark(markerName);
}

void SensorSystemLolec::setSeperator(const QString& seperator)
{
    QMutexLocker lock(&mMutex);
    mExporter.setSeperator(seperator);
}

void SensorSystemLolec::setExportSensors(const QList<QString> &items)
{
    QMutexLocker lock(&mMutex);
    mExporter.clearElements();
    foreach(QString item, items)
        mExporter.addElement(item, true);
}

void SensorSystemLolec::setExportSensors2(const QList<QString> &items, const QString &sensitiveSensor)
{
    QMutexLocker lock(&mMutex);
    mExporter.clearElements();
    foreach(QString item, items)
        mExporter.addElement(item, item == sensitiveSensor);
}

void SensorSystemLolec::exportTrace(const QString& filename)
{
    QMutexLocker lock(&mMutex);
    mExporter.exportTrace(filename);
}

void SensorSystemLolec::setStartAtZero(bool startTraceAtZero)
{
    QMutexLocker lock(&mMutex);
    mExporter.setStartAtZero(startTraceAtZero);
}


void SensorSystemLolec::setExportRange(const QString &startMarker, const QString &endMarker)
{
    QMutexLocker lock(&mMutex);
    mExporter.setMarkerRange(startMarker, endMarker);
}
