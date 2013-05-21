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

#ifndef POLYGONPORT_H
#define POLYGONPORT_H

#include "DefaultPortsGlobal.h"
#include <QList>
#include <QPolygonF>
#include <opencv2/core/core.hpp>

namespace port
{
    class DEFAULTPORTSSHARED_EXPORT Polygon : public Port
    {
    public:
        static QVariant variant(const QPolygonF& r);
        static QPolygonF fromVariant(const QVariant& v);
        static std::vector<cv::Point> pointList(const QPolygonF& poly);
    protected:
        Polygon();
    protected:
        QString toString(const QVariant &var) const;
        QVariant fromString(const QString &str) const;
        QVariant constrainedValue(const QVariant &var) const;
        QVariant toSimpleType(const QVariant& var) const;
    };
}

namespace out
{
    class DEFAULTPORTSSHARED_EXPORT Polygon : public port::Polygon
    {
    public:
        void send(const QPolygonF& poly);
        void send(const std::vector<cv::Point>& pointList);
    };
}

namespace in
{
    class DEFAULTPORTSSHARED_EXPORT Polygon : public port::Polygon
    {
    public:
        QPolygonF getValue();
        std::vector<cv::Point> getValue2() { return pointList(getValue()); }
        inline operator QPolygonF() { return getValue(); }
        inline operator std::vector<cv::Point>() { return getValue2(); }
    };
}



#endif // POLYGONPORT_H
