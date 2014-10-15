// OFFIS Automation Framework
// Copyright (C) 2013-2014 OFFIS e.V.
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

#include "cannyfilter.h"
#include <opencv2/imgproc/imgproc.hpp>
REGISTER_FILTER(CannyFilter);

CannyFilter::CannyFilter()
{
    setName("Canny");
    setDesc("Canny edge filter.");
    setGroup("image/edge detection");

    mOut.setName("outImage");
    mOut.setDesc("Image output port");
    addOutputPort(mOut);

    mIn.setName("inImage");
    mIn.setDesc("Image input port");
    addInputPort(mIn);

    mThreshhold.setName("threshold");
    mThreshhold.setDesc("Lower threshold");
    mThreshhold.setDefault(50);
    mThreshhold.setRange(1,300);
    addInputPort(mThreshhold);

    mRatio.setName("ratio");
    mRatio.setDesc("The ratio between lower and upper threshold");
    mRatio.setDefault(3.0);
    mRatio.setRange(2, 3);
    addInputPort(mRatio);

    mUseL1Gradient.setName("fastNormalization");
    mUseL1Gradient.setDesc("Use the faster but less accurate L1 normalization instead of L2 normalization");
    mUseL1Gradient.setDefault(true);
    addInputPort(mUseL1Gradient);
}

void CannyFilter::execute()
{
    int thresh = mThreshhold.getValue();
    int thresh2 = mRatio.getValue()*thresh;
    bool useL2grad = !mUseL1Gradient.getValue();
    const GrayImage src = mIn;
    GrayImage dest;
    cv::Canny(src, dest,thresh, thresh2 , 3 , useL2grad);
    mOut.send(dest);
}
