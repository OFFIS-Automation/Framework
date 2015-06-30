// OFFIS Automation Framework
// Copyright (C) 2013-2015 OFFIS e.V.
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

#ifndef COLORPORT_H
#define COLORPORT_H

#include "DefaultPortsGlobal.h"
#include <opencv2/core/core.hpp>
namespace port
{
    class DEFAULTPORTSSHARED_EXPORT Color : public Port
    {
    public:
        static QColor rgb(const cv::Vec3b& vec);
        static QColor rgba(const cv::Vec4b& vec);
        static QColor bgr(const cv::Vec3b& vec);
        static QColor bgra(const cv::Vec4b& vec);
        static cv::Vec3b rgb(const QColor& color);
        static cv::Vec4b rgba(const QColor& color);
        static cv::Vec3b bgr(const QColor& color);
        static cv::Vec4b bgra(const QColor& color);
    protected:
        Color();
        QString toString(const QVariant &var) const;
        QVariant fromString(const QString &str) const;
        QVariant constrainedValue(const QVariant &var) const;
    };
}

namespace out
{
    class DEFAULTPORTSSHARED_EXPORT Color : public port::Color
    {
    public:
        void send(const QColor& color);
        void sendRgb(const cv::Vec3b& rgb);
        void sendRgba(const cv::Vec4b& rgba);
        void sendBgr(const cv::Vec3b& rgb);
        void sendBgra(const cv::Vec4b& bgra);
        void send(uchar red, uchar green, uchar blue, uchar alpha = 0xFF);
    };
}

namespace in
{
    class DEFAULTPORTSSHARED_EXPORT Color : public port::Color
    {
    public:
        void setDefault(QColor value);
        QColor getValue();
        cv::Vec3b bgr();
        cv::Vec4b bgra();
        cv::Vec3b rgb();
        cv::Vec4b rgba();
        uchar red();
        uchar green();
        uchar blue();
        uchar alpha();
        inline operator QColor() { return getValue(); }

    };
}

#endif // COLORPORT_H
