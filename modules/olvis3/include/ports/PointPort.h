// OFFIS Automation Framework
// Copyright (C) 2013-2016 OFFIS e.V.
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

#ifndef POINTPORT_H
#define POINTPORT_H

#include "DefaultPortsGlobal.h"
#include <QPoint>
#include <opencv2/core.hpp>


namespace port
{
    class DEFAULTPORTSSHARED_EXPORT Point : public OverlayPort
    {
    public:
        static QPointF qpoint(const cv::Point2d& r);
        static QPointF qpoint(const cv::Point& r);
        static QPointF qpoint(const cv::Point2f& r);
        static cv::Point2d point(const QPointF& r);
        inline void setDefault(const cv::Point2d& value) { setDefault(qpoint(value)); }
        void setDefault(const QPointF& Point);
        inline void setMax(const cv::Point2d& value) { setMax(qpoint(value)); }
        void setMax(const QPointF& Point);
    protected:
        Point();
        QString toString(const QVariant &var) const;
        QVariant fromString(const QString &str) const;
        QVariant constrainedValue(const QVariant &var) const;
        QPointF mMax;
    };
}

namespace out
{
    class DEFAULTPORTSSHARED_EXPORT Point : public port::Point
    {
    public:
        inline void send(const cv::Point2d& value) { send(qpoint(value)); }
        void send(const QPointF& point);
    };
}

namespace in
{
    class DEFAULTPORTSSHARED_EXPORT Point : public port::Point
    {
    public:
        cv::Point2d getValue();
        QPointF getValue2();
        inline operator cv::Point2d() { return getValue(); }
        inline operator QPointF() { return getValue2(); }
    };
}

#endif // POINTPORT_H
