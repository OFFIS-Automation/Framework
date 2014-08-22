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

#include "SensorDataWriter.h"
#include <QFile>
#include <QCoreApplication>
#include <SensorDataSystem.h>
#include <QDebug>
SensorDataWriter::SensorDataWriter(QObject *parent) :
    QThread(parent)
{
    mStop = false;
    mCurrentId = 0;
    mMarkId = 0;
    mCurrentBuffer = &mBuffer[mCurrentId];
    start(IdlePriority);
}

SensorDataWriter& SensorDataWriter::instance()
{
    static QMutex mutex;
    QMutexLocker lock(&mutex);
    static SensorDataWriter writer;
    if(!writer.isRunning())
        writer.start();
    return writer;
}

SensorDataWriter::~SensorDataWriter()
{
    mMutex.lock();
    mStop = true;
    mMutex.unlock();
    wait();
}

QStringList SensorDataWriter::marker()
{
    QMutexLocker lock(&mDataMutex);
    return mMarker;
}

QStringList SensorDataWriter::tracedItems()
{
    QMutexLocker lock(&mDataMutex);
    return mItemNames;
}

void SensorDataWriter::add(const QString &item)
{
    QMutexLocker lock(&mDataMutex);
    if(mItemNames.contains(item))
        return;
    DataConsumer& c = SensorSystemInterface::consumer(item);
    connect(&c, SIGNAL(updated(QVariant,double,int)), SLOT(newData(QVariant,double,int)), Qt::DirectConnection);
    mItemNames << item;
    emit itemTraceAdded(item);
}

void SensorDataWriter::clear()
{
    QMutexLocker lock(&mDataMutex);
    foreach(QString item, mItemNames)
    {
        DataConsumer& c = SensorSystemInterface::consumer(item);
        c.disconnect(this);
        emit itemTraceRemoved(item);
    }
    mItemNames.clear();
}

void SensorDataWriter::remove(const QString &item)
{
    QMutexLocker lock(&mDataMutex);
    if(!mItemNames.contains(item))
        return;
    DataConsumer& c = SensorSystemInterface::consumer(item);
    c.disconnect(this);
    mItemNames.removeAll(item);
    emit itemTraceRemoved(item);

}

void SensorDataWriter::newData(const QVariant &data, double timestamp, int id)
{
    if(data.userType() == QMetaType::Void|| data.userType() >= QMetaType::User)
        return;

    QMutexLocker lock(&mMutex);
    mCurrentBuffer->append(Buffer(id, timestamp, data));
}

QString SensorDataWriter::mark(const QString& markName)
{
    double ts = SensorSystemInterface::currentTimestamp();
    QString name;
    {
        QMutexLocker lock(&mDataMutex);
        name = QString("%1: %2").arg(mMarkId++).arg(markName);
        mMarker << name;
    }
    newData(QVariant(name), ts, 0);
    qDebug() << "marker added:" << name;
    emit markerAdded(name);
    return name;
}

void SensorDataWriter::run()
{
    QFile file(QCoreApplication::applicationDirPath() + "/sensorSystem.dat");
    file.open(QFile::WriteOnly);
    QDataStream stream(&file);
    while(!mStop)
    {
        sleep(1);
        mMutex.lock();
        QLinkedList<Buffer>* buffer = mCurrentBuffer;
        mCurrentId = (mCurrentId+1) % 2;
        mCurrentBuffer = &mBuffer[mCurrentId];
        mMutex.unlock();
        foreach(const Buffer& item, *buffer)
        {
            stream << item.id << item.timestamp << item.data;
        }
        buffer->clear();
        file.flush();
    }
}


