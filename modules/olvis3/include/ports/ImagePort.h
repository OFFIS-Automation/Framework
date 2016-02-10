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

#ifndef IMAGEPORT_H
#define IMAGEPORT_H

#include "DefaultPortsGlobal.h"
#include <opencv2/core.hpp>
#include "GrayImage.h"
#include "RgbImage.h"
#include "RgbaImage.h"


namespace port
{
    class DEFAULTPORTSSHARED_EXPORT Image : public Port
    {
    public:
        static QVariant variant(const cv::Mat& mat);
        static const cv::Mat fromVariant(const QVariant& var);
    protected:
        Image(int typeId, const QString& name);
        QString toString(const QVariant &var) const;
        QVariant fromString(const QString &str) const;
        QVariant constrainedValue(const QVariant &var) const;
        QVariant toSimpleType(const QVariant& var) const;
    };
}
namespace out
{
    class DEFAULTPORTSSHARED_EXPORT Image : public port::Image
    {
    public:
        Image();
        virtual void send(const cv::Mat& image);

    };

    class DEFAULTPORTSSHARED_EXPORT GrayImage : public port::Image
    {
    public:
        GrayImage();
        virtual void send(const ::GrayImage& image);

    };

    class DEFAULTPORTSSHARED_EXPORT RgbImage : public port::Image
    {
    public:
        RgbImage();
        virtual void send(const ::RgbImage& image);

    };

    class DEFAULTPORTSSHARED_EXPORT RgbaImage : public port::Image
    {
    public:
        RgbaImage();
        virtual void send(const ::RgbaImage& image);

    };
}

namespace in
{
    class DEFAULTPORTSSHARED_EXPORT Image : public port::Image
    {
    public:
        Image();
        const cv::Mat getValue();
        inline operator const cv::Mat() { return getValue(); }

    };

    class DEFAULTPORTSSHARED_EXPORT GrayImage : public port::Image
    {
    public:
        GrayImage();
        const ::GrayImage getValue();
        inline operator const ::GrayImage() { return getValue(); }
    };

    class DEFAULTPORTSSHARED_EXPORT RgbImage : public port::Image
    {
    public:
        RgbImage();
        const ::RgbImage getValue();
        inline operator const ::RgbImage() { return getValue(); }
    };

    class DEFAULTPORTSSHARED_EXPORT RgbaImage : public port::Image
    {
    public:
        RgbaImage();
        const ::RgbaImage getValue();
        inline operator const ::RgbaImage() { return getValue(); }
    };
}


#endif // IMAGEPORT_H
