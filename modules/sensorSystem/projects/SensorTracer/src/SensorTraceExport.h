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

#ifndef SENSORTRACEEXPORT_H
#define SENSORTRACEEXPORT_H

#include "SensorTracer_global.h"

#include <QString>
#include <QMap>
#include <QSet>
#include <QVariant>

class SENSORTRACERSHARED_EXPORT SensorTraceExport : public QObject
{
public:
    SensorTraceExport();
    void addElement(const QString& id, bool addEntry);
    void setSeperator(const QString& seperator);
    void exportTrace(const QString& filename);
    void setStartAtZero(bool startAtZero);
    void setMarkerRange(QString startMarker, QString endMarker);
    void clearElements();
protected:
    QString convert(const QVariant& var);
    QString mSeperator;
    QMap<int, QVariant> mLastElements;
    QList<int> mIds;
    QSet<int> mWriteIds;
    QString mStartMarker, mEndMarker;
    bool mStartAtZero;
};

#endif // SENSORTRACEEXPORT_H
