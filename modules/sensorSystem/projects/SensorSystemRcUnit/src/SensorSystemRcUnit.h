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

#ifndef SENSORSYSTEMRCUNIT_H
#define SENSORSYSTEMRCUNIT_H

#include <QStringList>
#include <QObject>
#include <SensorDataWriter.h>
#include <SensorTraceExport.h>
#include <rc/types/UserRcUnit.h>

class SensorSystemRcUnit : public UserRcUnit
{
    Q_OBJECT
public:
    SensorSystemRcUnit();
    virtual RcFlagDefinitions rcFlagDefinitions() const { return RcFlagDefinitions(); }
    virtual QVariantList rcFlags() { return QVariantList();}
public slots:
    QList<QString> getAllSensors();
    QList<QString> getActiveSensors();

    void addToTrace(const QString& sensorName);
    void removeFromTrace(const QString& sensorName);
    QList<QString> getTracedSensors();

    QString addMarker(const QString& markerName);
    void setSeperator(const QString& seperator);
    void setExportSensors(const QList<QString>& items);
    void setExportSensors2(const QList<QString> &items, const QString &sensitiveSensor);
    void setExportRange(const QString& startMarker, const QString& endMarker);
    void exportTrace(const QString &filename);
    void setStartAtZero(bool startTraceAtZero);
private:
    SensorDataWriter& mWriter;
    QMutex mMutex;
    SensorTraceExport mExporter;
    QString mExportSensors;

};

#endif // SENSORSYSTEMRCUNIT_H
