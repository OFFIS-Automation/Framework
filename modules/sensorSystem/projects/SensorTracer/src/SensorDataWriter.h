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

#ifndef SENSORDATAWRITER_H
#define SENSORDATAWRITER_H

#include "SensorTracer_global.h"

#include <QThread>
#include <QMutex>
#include <QVariant>
#include <QLinkedList>
#include <QStringList>
#include <QSet>

class SENSORTRACERSHARED_EXPORT SensorDataWriter : public QThread
{
    Q_OBJECT
    Q_DISABLE_COPY(SensorDataWriter)
    explicit SensorDataWriter(QObject *parent = 0);
public:
    static SensorDataWriter& instance();
    virtual ~SensorDataWriter();
    QStringList marker();
    QStringList tracedItems();

signals:
    void itemTraceAdded(const QString& name);
    void itemTraceRemoved(const QString& name);
    void markerAdded(const QString& name);
public slots:
    QString mark(const QString &markName);
    void add(const QString& item);
    void clear();
    void remove(const QString& item);
    void newData(const QVariant& data, double timestamp, int id);
protected:
    void run();
    bool mStop;
    QMutex mMutex, mDataMutex;

    struct Buffer
    {
        Buffer(int _id, double _timestamp, const QVariant& _data) : id(_id), timestamp(_timestamp), data(_data) {}
        int id;
        double timestamp;
        QVariant data;
    };
    QLinkedList<Buffer> mBuffer[2], *mCurrentBuffer;
    int mCurrentId;
    int mMarkId;
    QStringList mMarker;
    QStringList mItemNames;

};

#endif // SENSORDATAWRITER_H
