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

#include <ports/ImagePort.h>
#include <opencv2/imgproc.hpp>

Q_DECLARE_METATYPE(cv::Mat)
Q_DECLARE_METATYPE(Image)

QVariant port::Image::variant(const cv::Mat &val)
{
    return QVariant::fromValue<cv::Mat>(val);
}

const cv::Mat port::Image::fromVariant(const QVariant &val)
{
    return val.value<cv::Mat>();
}

port::Image::Image(int typeId, const QString &typeName) : Port(typeId, typeName, true)
{
}

QString port::Image::toString(const QVariant &) const
{
    return QString();
}

QVariant port::Image::fromString(const QString &) const
{
    return QVariant();
}

QVariant port::Image::constrainedValue(const QVariant &var) const
{
    return var;
}

QVariant port::Image::toSimpleType(const QVariant &var) const
{
    cv::Mat mat = fromVariant(var);
    return QSizeF(mat.cols, mat.rows);
}

in::Image::Image() : port::Image(qMetaTypeId<cv::Mat>(), "Image")
{
    setIcon(QImage(":/defaultPorts/images/image.png"));
    addCompatiblePort(qMetaTypeId< ::Image>(), QObject::tr("Image is converted if neccessary"));
    setName("imageIn");
}

const Image in::Image::getValue()
{
    cv::Mat src = fromVariant(getRawValue());
    return ::Image(src);
}

out::Image::Image() : port::Image(qMetaTypeId< ::Image>(), "Image")
{
    setIcon(QImage(":/defaultPorts/images/image.png"));
    setName("imageOut");
}

void out::Image::send(const ::Image &image)
{
    sendRaw(variant(image));
}

