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

#include <ports/PolygonPort.h>
#include <QStringList>
#include <core/PortInfo.h>
#include <ports/PointPort.h>

Q_DECLARE_METATYPE(QPolygonF);

port::Polygon::Polygon() : Port(qMetaTypeId<QPolygonF>(), "Polygon")
{
    setIcon(QImage(":/defaultPorts/images/shape.png"));
}

QVariant port::Polygon::variant(const QPolygonF &p)
{
    return qVariantFromValue<QPolygonF>(p);
}

QPolygonF port::Polygon::fromVariant(const QVariant &v)
{
    return v.value<QPolygonF>();
}

std::vector<cv::Point> port::Polygon::pointList(const QPolygonF &poly)
{
    std::vector<cv::Point> list(poly.size());
    for(int i=0; i<poly.size(); i++)
        list[i] = Point::point(poly[i]);
    return list;
}


QString port::Polygon::toString(const QVariant&) const
{
    return QString();
}

QVariant port::Polygon::fromString(const QString&) const
{
    return QVariant();
}

QVariant port::Polygon::constrainedValue(const QVariant &var) const
{
    return var;
}

QVariant port::Polygon::toSimpleType(const QVariant &var) const
{
    QList<QVariant> list;
    QPolygonF poly = fromVariant(var);
    foreach(QPointF p, poly)
    {
        QList<QVariant> pl;
        pl << p.x();
        pl << p.y();
        list << pl;
    }
    QVariantMap map;
    map["type"] = "Polygon";
    map["points"] = list;
    return map;
}

QPolygonF in::Polygon::getValue()
{
    return fromVariant(getRawValue());
}

void out::Polygon::send(const QPolygonF& value)
{
    sendRaw(variant(value));
}

void out::Polygon::send(const std::vector<cv::Point> &pointList)
{
    QPolygonF poly;
    for(uint j=0;j<pointList.size(); j++)
        poly << (port::Point::qpoint(pointList[j]));
    send(poly);
}
