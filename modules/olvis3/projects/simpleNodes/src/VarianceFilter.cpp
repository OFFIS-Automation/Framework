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

#include "VarianceFilter.h"
#include <opencv2/imgproc.hpp>

REGISTER_FILTER(VarianceFilter);
VarianceFilter::VarianceFilter()
{
    setName("Variance");
    setDesc(QObject::tr("Calculates the variance inside the image or region of interest"));
    setGroup("image/analysis");

    mIn.setName("imageIn");
    mIn.setDesc(QObject::tr("Image input"));
    addInputPort(mIn);

    mVarianceOut.setName("variance");
    addOutputPort(mVarianceOut);

    mMeanOut.setName("mean grayscale");
    addOutputPort(mMeanOut);

    mRoiIn.setMode(OptionalPortMode);
    mRoiIn.setName("roi");
    addInputPort(mRoiIn);
}

void VarianceFilter::execute()
{
    cv::Mat src = mIn;

    if(mRoiIn.hasValue())
    {
        cv::Size size = src.size();
        QRectF imageRect(0,0, size.width, size.height);
        QRectF roi = mRoiIn;
        roi = roi.intersected(imageRect);
        if(roi.isValid())
            src = src(port::Rect::rect(roi));
    }
    cv::Scalar mean, stdDev;
    cv::meanStdDev(src, mean, stdDev);
    double variance = stdDev.val[0]*stdDev.val[0];
    mVarianceOut.send(variance);
    mMeanOut.send(mean[0]);
}
