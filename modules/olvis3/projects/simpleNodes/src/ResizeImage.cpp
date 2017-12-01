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

#include "ResizeImage.h"
#include <opencv2/imgproc.hpp>
#include <QDebug>

REGISTER_FILTER(ResizeImage);
ResizeImage::ResizeImage()
{
    setName("Resize");
    setDesc(QObject::tr("Resizes an image to the given size. If the filter should constrain to with and height both, the width is in favour"));
    setGroup("image/reshape");

    // Input
    addInputPort(mIn);

    mAspectRatio.setName("aspectRatioMode");
    mAspectRatio.setDesc(QObject::tr("how to deal with aspect ratios"));
    mAspectRatio.addChoice(IgnoreAspectRatio, tr("Ignore aspect ratio"));
    mAspectRatio.addChoice(KeepAspectRatioByWidth, tr("Keep aspect ratio, ignore height"));
    mAspectRatio.addChoice(KeepAspectRatioByHeight, tr("Keep aspect ratio, ignore width"));
    mAspectRatio.setDefault(IgnoreAspectRatio);
    mAspectRatio.setIcon(QImage(":/SimpleNodes/constrain.png"));
    addInputPort(mAspectRatio);

    mWidth.setName("width");
    mWidth.setDesc(QObject::tr("Width of the output image"));
    addInputPort(mWidth);

    mHeight.setName("height");
    mHeight.setDesc(QObject::tr("Height of the output image"));
    addInputPort(mHeight);

    // Output
    addOutputPort(mOut);
}

void ResizeImage::execute()
{
    const cv::Mat src = mIn;
    cv::Mat out = src.clone();

    if(mWidth.hasValue() && mHeight.hasValue()){
        cv::Size size;


        if(mAspectRatio.getValue() == KeepAspectRatioByWidth){
            size = cv::Size(mWidth, ((float)mWidth / (float)src.size().width) * src.size().height);
        } else if(mAspectRatio.getValue() == KeepAspectRatioByHeight) {
            size = cv::Size(((float)mHeight / (float)src.size().height) * src.size().width , mHeight);
        } else {
            size = cv::Size(mWidth, mHeight);
        }
        cv::resize(src, out, size, 0, 0);
    }

    mOut.send(out);
}
