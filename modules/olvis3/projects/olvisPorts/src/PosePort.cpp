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

#include <ports/PosePort.h>
#include <QStringList>

Q_DECLARE_METATYPE(Pose2d);

Pose2d port::Pose2d::fromVariant(const QVariant &var)
{
    return var.value< ::Pose2d>();
}

QVariant port::Pose2d::variant(const ::Pose2d &pose)
{
    return qVariantFromValue< ::Pose2d>(pose);
}


port::Pose2d::Pose2d() : Port(qMetaTypeId< ::Pose2d>(), "Pose2d")
{
    setIcon(QImage(":/defaultPorts/images/pose.png"));
}


void port::Pose2d::setDefault(const ::Pose2d &pose)
{
    setDefaultValueRaw(variant(pose));
}

QString port::Pose2d::toString(const QVariant& var) const
{
    ::Pose2d pose = fromVariant(var);
    return QString("%1, %2, %3").arg(pose.x).arg(pose.y).arg(pose.phi);
}

QVariant port::Pose2d::fromString(const QString& str) const
{
    QStringList p = str.split(",");
    if(p.length() < 3)
        return variant(::Pose2d());
    return variant(::Pose2d(p[0].toDouble(), p[1].toDouble(), p[2].toDouble()));
}

QVariant port::Pose2d::toSimpleType(const QVariant &var) const
{
    QVariantMap map;
    map["type"] = "Pose2d";
    ::Pose2d pose = fromVariant(var);
    map["x"] = pose.x;
    map["y"] = pose.y;
    map["phi"] = pose.phi;
    return map;
}
::Pose2d in::Pose2d::getValue()
{
    return fromVariant(getRawValue());
}

void out::Pose2d::send(const ::Pose2d &value)
{
    sendRaw(variant(value));
}

