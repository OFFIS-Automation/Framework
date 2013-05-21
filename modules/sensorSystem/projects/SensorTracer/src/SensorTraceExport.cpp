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

#include "SensorTraceExport.h"

#include <QCoreApplication>
#include <QFile>
#include <QRectF>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QTextStream>
#include <QStringList>
#include <QLineF>
#include <SensorDataSystem.h>
#include <QDebug>

SensorTraceExport::SensorTraceExport() :
    mSeperator(","), mStartAtZero(false)
{
}
void SensorTraceExport::clearElements()
{
    mWriteIds.clear();
    mIds.clear();
}

void SensorTraceExport::addElement(const QString& item, bool addEntry)
{
    DataConsumer& c = SensorSystemInterface::consumer(item);
    mIds << c.numericIdentifier();
    if(addEntry)
        mWriteIds << c.numericIdentifier();
}

void SensorTraceExport::setSeperator(const QString &seperator)
{
    mSeperator = seperator;
}


void SensorTraceExport::setStartAtZero(bool startAtZero)
{
    mStartAtZero = startAtZero;
}

void SensorTraceExport::setMarkerRange(QString startMarker, QString endMarker)
{
    mStartMarker = startMarker;
    mEndMarker = endMarker;
}

void SensorTraceExport::exportTrace(const QString &filename)
{
    QString sourceName = QCoreApplication::applicationDirPath() + "/sensorSystem.dat";
    QFile sourceFile(sourceName);
    if(!sourceFile.open(QFile::ReadOnly))
    {
        qCritical() << tr("Could not open system trace file for reading");
        return;
    }
    QFile targetFile(filename);
    if(!targetFile.open(QFile::WriteOnly | QFile::Text))
    {
        qCritical() << tr("Could not open trace file for writing");
        return;
    }
    bool started = false;
    double offset = 0.0;
    QDataStream input(&sourceFile);
    QTextStream output(&targetFile);
    int id = -1;
    double timestamp = 0.0;
    QVariant data;
    bool startMarkerFound = mStartMarker.isEmpty();
    while(!input.atEnd())
    {
        input >> id >> timestamp >> data;
        if(!data.isValid())
            break;
        if(id == 0)
        {
            qDebug() << data;
            QString test = data.toString();
            if(!startMarkerFound)
            {
                if(data.toString() == mStartMarker)
                    startMarkerFound = true;
            }
            if(data.toString() == mEndMarker)
            {
                break;
            }

        }
        if(!startMarkerFound)
            continue;
        if(!mIds.contains(id))
            continue;
        mLastElements[id] = data;
        if(!mWriteIds.contains(id))
            continue;
        if(!started)
        {
            if(mLastElements.size() != mIds.size())
                continue;
            started = true;
            if(mStartAtZero)
                offset = timestamp;
        }
        output << timestamp-offset;
        foreach(int id, mIds)
        {
            output << mSeperator << convert(mLastElements[id]);
        }
        output << endl;
    }
}


QString SensorTraceExport::convert(const QVariant &var)
{
    QStringList strList;
    switch(var.type())
    {
    case QVariant::String:
    case QVariant::ByteArray:
        return QString("\"%1\"").arg(var.toString().replace("\"","\\\""));
    case QVariant::Bool:
        return var.toChar();
    case QVariant::Line:
    case QVariant::LineF:
        return QString("%2%1%3").arg(mSeperator).arg(convert(var.toLineF().p1()))
                .arg(convert(var.toLineF().p2()));
    case QVariant::Point:
    case QVariant::PointF:
        return QString("%2%1%3").arg(mSeperator).arg(var.toPointF().x()).arg(var.toPointF().y());
    case QVariant::Size:
    case QVariant::SizeF:
        return QString("%2%1%3").arg(mSeperator).arg(var.toSizeF().width()).arg(var.toSizeF().height());
    case QVariant::Vector2D:
        return QString("%2%1%3").arg(mSeperator).arg(var.value<QVector2D>().x()).arg(var.value<QVector2D>().y());
    case QVariant::Vector3D:
        return QString("%2%1%3%1%4").arg(mSeperator).arg(var.value<QVector3D>().x())
                .arg(var.value<QVector3D>().y()).arg(var.value<QVector3D>().z());
    case QVariant::Vector4D:
        return QString("%2%1%3%1%4%1%5").arg(mSeperator).arg(var.value<QVector4D>().x())
                .arg(var.value<QVector4D>().y()).arg(var.value<QVector4D>().z()).arg(var.value<QVector4D>().w());
    case QVariant::Rect:
    case QVariant::RectF:
        return QString("%1%2%1%3%1%4%1%5").arg(mSeperator)
                .arg(var.toRectF().x()).arg(var.toRectF().y())
                .arg(var.toRectF().width()).arg(var.toRectF().height());
    case QVariant::List:
    case QVariant::StringList:
        foreach(QVariant elem, var.toList())
            strList << convert(elem);
        return strList.join(mSeperator);
    default:
        return var.toString();
    }
}

