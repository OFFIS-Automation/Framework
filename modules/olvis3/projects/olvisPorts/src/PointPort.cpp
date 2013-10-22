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

#include <ports/PointPort.h>
#include <QStringList>

QPointF port::Point::qpoint(const cv::Point2d &r)
{
    return QPointF(r.x, r.y);
}

QPointF port::Point::qpoint(const cv::Point &r)
{
    return QPointF(r.x, r.y);
}

QPointF port::Point::qpoint(const cv::Point2f &r)
{
    return QPointF(r.x, r.y);
}

cv::Point2d port::Point::point(const QPointF &r)
{
    return cv::Point2d(r.x(), r.y());
}


port::Point::Point() : OverlayPort(QMetaType::QPointF, "Point")
{
    setIcon(QImage(":/defaultPorts/images/point.png"));
}


void port::Point::setDefault(const QPointF& value)
{
    setDefaultValueRaw(value);
}

void port::Point::setMax(const QPointF& value)
{
    mMax = value;
    setConstraint("max", value);
}

QString port::Point::toString(const QVariant& var) const
{
    QPoint r = var.toPoint();
    return QString("%1,%2").arg(r.x()).arg(r.y());
}

QVariant port::Point::fromString(const QString& str) const
{
    QStringList p = str.split(",");
    if(p.length() < 2)
        return QPointF();
    return QPointF(p[0].toDouble(), p[1].toDouble());
}


QVariant port::Point::constrainedValue(const QVariant &var) const
{
    if(mMax.isNull())
        return var;
    QPointF p = var.toPointF();
    p = QPointF(qMin(mMax.x(), p.x()), qMin(mMax.y(), p.y()));
    return QVariant(p);
}

cv::Point2d in::Point::getValue()
{
    return point(getValue2());
}

QPointF in::Point::getValue2()
{
    return getRawValue().toPointF();
}

void out::Point::send(const QPointF& value)
{
    sendRaw(value);
}

