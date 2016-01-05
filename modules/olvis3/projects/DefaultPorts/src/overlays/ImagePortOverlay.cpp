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

#include "ImagePortOverlay.h"
#include <ports/DepthMapPort.h>
#include <QMutexLocker>
#include <QPainter>
#include <QImage>

#include <QDebug>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include <opencv2/imgproc/imgproc.hpp>

ImagePortOverlay::ImagePortOverlay(QString name)
    : MainOverlay(name), mScale(1.0), mImage()
{
    mFixedAspectRatio = true;
    mRect = QRect(0, 0, 640, 480);
}

void ImagePortOverlay::writeCurrentConfig(QXmlStreamWriter &writer)
{
    RectOverlay::writeCurrentConfig(writer);
    writer.writeEmptyElement("scale");
    writer.writeAttribute("value", QString("%1").arg(mScale));
}

void ImagePortOverlay::readElement(QXmlStreamReader &reader)
{
    if (reader.name() == "scale") {
        mScale = reader.attributes().value("value").toString().toDouble();
    } else {
        RectOverlay::readElement(reader);
    }
}

void ImagePortOverlay::paintContent(QPainter& p)
{
    mMutex.lock();

    if (mLastValue.isValid()) {
        cv::Mat mat = port::Image::fromVariant(mLastValue);
        if(mat.data == 0) // try depth map
        {
            DepthMap dm = port::DepthMap::fromVariant(mLastValue);
            if(dm.data != 0)
                mat = dm.toImage();
        }
        if (mat.size().width != 0 && mat.size().height != 0) {
            QImage::Format format = QImage::Format_RGB32;
            //QImage::Format format = QImage::Format_RGB16;
            if (mat.type() == CV_8UC1) {
                cv::cvtColor(mat, converted, CV_GRAY2BGRA, 4);
                //cv::cvtColor(mat, converted, CV_GRAY2BGR565, 2);
            } else if (mat.type() == CV_8UC3) {
                cv::cvtColor(mat, converted, CV_BGR2BGRA);
                //cv::cvtColor(mat, converted, CV_BGR2BGR565);
            } else if(mat.type() == CV_8UC4) {
                converted = mat;
                // all is good
            } else {
                qCritical() << "VideoDisplayWidget: unknown image format received" << mat.type();
            }
            mLastValue = QVariant();
            mImage = QImage(converted.data, converted.size().width, converted.size().height, converted.step, format);

            QSize imageSize = mImage.size();
            if (mState == Scaling){
                mScale = mRect.width() / (double) imageSize.width();
            } else {
                mRect.setSize(imageSize * mScale);
            }
            //setAspectRatio(mImage.width() / (double) mImage.height());
        } else {
            qWarning() << tr("VideoDisplay: image without dimensions received");
        }
    }
    mMutex.unlock();

    QRect rect = (mRect.isEmpty() ? QRect(0, 0, 640, 480) : mRect);

    mPen.setColor(Qt::black);
    p.setPen(mPen);
    if (mImage.isNull())
        p.setBrush(QColor(0, 0, 0, 128));
    p.drawRect(rect);
    if (!mImage.isNull()) {
        p.drawImage(rect, mImage);
    }
}

QSize ImagePortOverlay::size()
{
    return mRect.size();
}
