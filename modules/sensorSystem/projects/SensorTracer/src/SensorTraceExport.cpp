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
#include <QDataStream>

SensorTraceExport::SensorTraceExport() :
    mSeperator(","), mStartAtZero(false), mHeader(false), mOnlyValidLines(true)
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
    mNames << item;
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

bool SensorTraceExport::exportTrace(const QString &filename, QString& errorMsg)
{
    QString sourceName = QCoreApplication::applicationDirPath() + "/sensorSystem.dat";
    QFile sourceFile(sourceName);
    if(!sourceFile.open(QFile::ReadOnly))
    {
        errorMsg = tr("Could not open system trace file for reading");
        qCritical() << errorMsg;
        return false;
    }
    QFile targetFile(filename);
    if(!targetFile.open(QFile::WriteOnly | QFile::Text))
    {
        errorMsg = tr("Could not open trace file for writing");
        qCritical() << errorMsg;
        return false;
    }
    bool started = false;
    double offset = 0.0;
    QDataStream input(&sourceFile);
    QTextStream output(&targetFile);
    int id = -1;
    double timestamp = 0.0;
    output << "Time";
    mLastElements.clear();
    foreach(int id, mIds)
        mLastElements[id] = QVariant();
    foreach(QString name, mNames)
    {
        output << mSeperator << name.replace(mSeperator, "-");
    }
    output << endl;
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
        if(mOnlyValidLines)
        {
            bool allValid = true;
            foreach(int id, mIds)
            {
                if(!mLastElements.contains(id) || !mLastElements[id].isValid())
                {
                    allValid = false;
                    break;
                }
            }
            if(!allValid)
                continue;
        }
        output << timestamp-offset;
        foreach(int id, mIds)
        {
            output << mSeperator << convertToString(mLastElements[id], mSeperator);
        }
        output << endl;
    }
    return true;
}

bool SensorTraceExport::exportTrace(const QString &filename)
{
	QString errorMessage;

	return exportTrace(filename, errorMessage);
}

QString SensorTraceExport::convertToString(const QVariant &var, const QString& seperator)
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
        return QString("%2%1%3").arg(seperator).arg(convertToString(var.toLineF().p1(), seperator))
                .arg(convertToString(var.toLineF().p2(), seperator));
    case QVariant::Point:
    case QVariant::PointF:
        return QString("%2%1%3").arg(seperator).arg(var.toPointF().x()).arg(var.toPointF().y());
    case QVariant::Size:
    case QVariant::SizeF:
        return QString("%2%1%3").arg(seperator).arg(var.toSizeF().width()).arg(var.toSizeF().height());
    case QVariant::Vector2D:
        return QString("%2%1%3").arg(seperator).arg(var.value<QVector2D>().x()).arg(var.value<QVector2D>().y());
    case QVariant::Vector3D:
        return QString("%2%1%3%1%4").arg(seperator).arg(var.value<QVector3D>().x())
                .arg(var.value<QVector3D>().y()).arg(var.value<QVector3D>().z());
    case QVariant::Vector4D:
        return QString("%2%1%3%1%4%1%5").arg(seperator).arg(var.value<QVector4D>().x())
                .arg(var.value<QVector4D>().y()).arg(var.value<QVector4D>().z()).arg(var.value<QVector4D>().w());
    case QVariant::Rect:
    case QVariant::RectF:
        return QString("%1%2%1%3%1%4%1%5").arg(seperator)
                .arg(var.toRectF().x()).arg(var.toRectF().y())
                .arg(var.toRectF().width()).arg(var.toRectF().height());
    case QVariant::List:
    case QVariant::StringList:
        foreach(QVariant elem, var.toList())
            strList << convertToString(elem, seperator);
        return strList.join(seperator);
    case QVariant::Invalid:
        return "@INVALID";
    default:
        return var.toString();
    }
}

QList<QVariant> SensorTraceExport::convertToDouble(const QVariant &var)
{
    QList<QVariant> dblList;
    switch(var.type())
    {
    case QVariant::String:
    case QVariant::ByteArray:
    case QVariant::Bool:
        dblList << var.toDouble();
        break;
    case QVariant::Line:
    case QVariant::LineF:
        dblList << convertToDouble(var.toLineF().p1());
        dblList << convertToDouble(var.toLineF().p2());
        break;
    case QVariant::Point:
    case QVariant::PointF:
        dblList << var.toPointF().x();
        dblList << var.toPointF().y();
        break;
    case QVariant::Size:
    case QVariant::SizeF:
        dblList << var.toSizeF().width();
        dblList << var.toSizeF().height();
        break;
    case QVariant::Vector2D:
        dblList << var.value<QVector2D>().x();
        dblList << var.value<QVector2D>().y();
        break;
    case QVariant::Vector3D:
        dblList << var.value<QVector3D>().x();
        dblList << var.value<QVector3D>().y();
        dblList << var.value<QVector3D>().z();
        break;
    case QVariant::Vector4D:
        dblList << var.value<QVector4D>().x();
        dblList << var.value<QVector4D>().y();
        dblList << var.value<QVector4D>().z();
        dblList << var.value<QVector4D>().w();
        break;
    case QVariant::Rect:
    case QVariant::RectF:
        dblList << convertToDouble(var.toRectF().topLeft());
        dblList << convertToDouble(var.toRectF().bottomRight());
        dblList << convertToDouble(var.toSizeF());
        break;
    case QVariant::List:
    case QVariant::StringList:
        foreach(QVariant elem, var.toList())
            dblList << convertToDouble(elem);
        break;
    default:
        dblList << var.toDouble();
        break;
    }
    return dblList;
}
