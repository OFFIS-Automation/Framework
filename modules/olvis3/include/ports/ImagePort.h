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

#ifndef IMAGEPORT_H
#define IMAGEPORT_H

#include "DefaultPortsGlobal.h"
#include <opencv2/core.hpp>
#include "Image.h"

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
        virtual void send(const ::Image& image);
    };
}

namespace in
{
    class DEFAULTPORTSSHARED_EXPORT Image : public port::Image
    {
    public:
        Image();
        const ::Image getValue();
        inline operator const ::Image() { return getValue(); }
    };

}


#endif // IMAGEPORT_H
