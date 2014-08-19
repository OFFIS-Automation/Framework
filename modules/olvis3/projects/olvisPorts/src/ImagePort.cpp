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

#include <ports/ImagePort.h>
#include <opencv2/imgproc/imgproc.hpp>

Q_DECLARE_METATYPE(cv::Mat)
Q_DECLARE_METATYPE(GrayImage)
Q_DECLARE_METATYPE(RgbImage)
Q_DECLARE_METATYPE(RgbaImage)

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
    addCompatiblePort(qMetaTypeId< ::GrayImage>());
    addCompatiblePort(qMetaTypeId< ::RgbaImage>());
    addCompatiblePort(qMetaTypeId< ::RgbImage>());
    setName("imageIn");
}

const cv::Mat in::Image::getValue()
{
    return fromVariant(getRawValue());
}

in::GrayImage::GrayImage() : port::Image(qMetaTypeId< ::GrayImage>(), "GrayImage")
{
    setIcon(QImage(":/defaultPorts/images/grayimage.png"));
    addCompatiblePort(qMetaTypeId<cv::Mat>(), QObject::tr("Image is converted to grayscale if neccessary"));
    addCompatiblePort(qMetaTypeId< ::RgbImage>(), QObject::tr("Image is converted to grayscale"));
    addCompatiblePort(qMetaTypeId< ::RgbaImage>(), QObject::tr("Image is converted to grayscale"));
    setName("imageIn");
}

const GrayImage in::GrayImage::getValue()
{
    cv::Mat src = fromVariant(getRawValue());
    return ::GrayImage(src);
}


in::RgbImage::RgbImage() : port::Image(qMetaTypeId< ::RgbImage>(), "RGBImage")
{
    setIcon(QImage(":/defaultPorts/images/rgbimage.png"));
    addCompatiblePort(qMetaTypeId<cv::Mat>(), QObject::tr("Image is converted to RGB if neccessary"));
    addCompatiblePort(qMetaTypeId< ::GrayImage>(), QObject::tr("Image is converted to RGB"));
    addCompatiblePort(qMetaTypeId< ::RgbaImage>(), QObject::tr("Image is converted to RGB"));
    setName("imageIn");
}

const RgbImage in::RgbImage::getValue()
{
    cv::Mat src = fromVariant(getRawValue());
    return ::RgbImage(src);
}


in::RgbaImage::RgbaImage() : port::Image(qMetaTypeId< ::RgbaImage>(), "RGBAImage")
{
    setIcon(QImage(":/defaultPorts/images/rgbimage.png"));
    addCompatiblePort(qMetaTypeId<cv::Mat>(), QObject::tr("Image is converted to RGBA if neccessary"));
    addCompatiblePort(qMetaTypeId< ::GrayImage>(), QObject::tr("Image is converted to RGBA"));
    addCompatiblePort(qMetaTypeId< ::RgbImage>(), QObject::tr("Image is converted to RGBA"));
    setName("imageIn");
}

const RgbaImage in::RgbaImage::getValue()
{
    cv::Mat src = fromVariant(getRawValue());
    return ::RgbaImage(src);
}


out::Image::Image() : port::Image(qMetaTypeId<cv::Mat>(), "Image")
{
    setIcon(QImage(":/defaultPorts/images/image.png"));
    setName("imageOut");
}

void out::Image::send(const cv::Mat &image_)
{
    cv::Mat image = image_;
    if(image.depth() != CV_8U)
        image_.convertTo(image, CV_8U);
    sendRaw(variant(image));
}

out::GrayImage::GrayImage() : port::Image(qMetaTypeId< ::GrayImage>(), "GrayImage")
{
    setIcon(QImage(":/defaultPorts/images/grayimage.png"));
    setName("imageOut");
}

void out::GrayImage::send(const ::GrayImage &image)
{
    sendRaw(variant(image));
}

out::RgbImage::RgbImage() : port::Image(qMetaTypeId< ::RgbImage>(), "RGBImage")
{
    setIcon(QImage(":/defaultPorts/images/rgbimage.png"));
    setName("imageOut");
}

void out::RgbImage::send(const ::RgbImage &image)
{
    sendRaw(variant(image));
}


out::RgbaImage::RgbaImage() : port::Image(qMetaTypeId< ::RgbaImage>(), "RGBAImage")
{
    setIcon(QImage(":/defaultPorts/images/rgbimage.png"));
    setName("imageOut");
}

void out::RgbaImage::send(const ::RgbaImage &image)
{
    sendRaw(variant(image));
}
