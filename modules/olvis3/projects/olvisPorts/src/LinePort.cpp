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

#include <ports/LinePort.h>
#include <core/PortInfo.h>



port::Line::Line() : Port(QVariant::LineF, "Line")
{
    setIcon(QImage(":/defaultPorts/images/chart_line.png"));
}


QString port::Line::toString(const QVariant& var) const
{
    QLineF line = var.toLineF();
    return QString("%1,%2->%3,%4").arg(line.x1()).arg(line.y1()).arg(line.x2()).arg(line.y2());
}

QVariant port::Line::fromString(const QString& str) const
{
    QStringList parts = str.split("->");
    if(parts.length() != 2)
        return QVariant();
    QStringList start = parts[0].split(",");
    QStringList end = parts[1].split(",");
    if(start.length() != 2 || end.length() != 2)
        return QVariant();
    QPointF startPoint = QPointF(start[0].toDouble(), start[1].toDouble());
    QPointF endPoint = QPointF(end[0].toDouble(), end[1].toDouble());
    return QVariant(QLineF(startPoint, endPoint));
}

QVariant port::Line::constrainedValue(const QVariant &var) const
{
    return var;
}

QVariant port::Line::toSimpleType(const QVariant &var) const
{
    QList<QVariant> list;
    QLineF line = var.toLineF();
    QVariantMap map;
    map["type"] = "Line";
    map["x1"] = line.x1();
    map["x2"] = line.x2();
    map["y1"] = line.y1();
    map["y2"] = line.y2();
    return map;
}

QLineF in::Line::getValue()
{
    return getRawValue().toLineF();
}


void out::Line::send(const QLineF& value)
{
    sendRaw(value);
}


