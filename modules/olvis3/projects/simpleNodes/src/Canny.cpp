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

#include "Canny.h"
#include <opencv2/imgproc.hpp>

REGISTER_FILTER(Canny);
Canny::Canny()
{
    setName("Canny");
    setDesc(QObject::tr("Canny edge filter<br>Input: 8C1 / 8C3"));
    setGroup("image/edge detection");

    mOut.setName("imageOut");
    mOut.setDesc(QObject::tr("Image output"));
    addOutputPort(mOut);

    mIn.setName("imageIn");
    mIn.setDesc(QObject::tr("Image input"));
    addInputPort(mIn);

    mThreshhold.setName("threshold");
    mThreshhold.setDesc(QObject::tr("Lower threshold"));
    mThreshhold.setDefault(50);
    mThreshhold.setRange(1,300);
    addInputPort(mThreshhold);

    mRatio.setName("ratio");
    mRatio.setDesc(QObject::tr("The ratio between lower and upper threshold"));
    mRatio.setDefault(3.0);
    mRatio.setRange(1, 10);
    addInputPort(mRatio);

    mUseL1Gradient.setName("fastNormalization");
    mUseL1Gradient.setDesc(QObject::tr("Use the faster but less accurate L1 normalization instead of L2 normalization"));
    mUseL1Gradient.setDefault(true);
    mUseL1Gradient.setVisibility(AdvancedPortVisibility);
    addInputPort(mUseL1Gradient);
}

void Canny::execute()
{
    int thresh = mThreshhold.getValue();
    int thresh2 = mRatio.getValue()*thresh;
    bool useL2grad = !mUseL1Gradient.getValue();

    const cv::Mat src = mIn;
    cv::Mat srcConverted = src.clone();
    ((Image *)&srcConverted)->convertToBit(CV_8U);

    cv::Mat dest;
    cv::Canny(srcConverted, dest,thresh, thresh2 , 3 , useL2grad);
    mOut.send(dest);
}
