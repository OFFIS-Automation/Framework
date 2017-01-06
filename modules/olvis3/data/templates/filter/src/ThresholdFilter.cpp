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

#include "ThresholdFilter.h"
#include <opencv2/imgproc.hpp>

REGISTER_FILTER(ThresholdFilter);


ThresholdFilter::ThresholdFilter()
{
    setName("Threshold");
    setDesc(tr("Thresholds an image"));
    setGroup("image/color");

    mOut.setName("imageOut");
    mOut.setDesc(tr("Image output"));
    addOutputPort(mOut);

    mIn.setName("imageIn");
    mIn.setDesc(tr("Image input"));
    addInputPort(mIn);

    mThreshold.setName("threshold");
    mThreshold.setDesc("Threshold value");
    mThreshold.setDefault(128);
    mThreshold.setRange(0, 255);
    addInputPort(mThreshold);

    mMode.setName("mode");
    mMode.setDesc(tr("Thresholding mode"));
    mMode.addChoice(cv::THRESH_BINARY, tr("Binary (value = value > threshold ? max_value : 0 )"));
    mMode.addChoice(cv::THRESH_BINARY_INV, tr("Binary inverted (value = value > threshold ? 0 : max_value)"));
    mMode.addChoice(cv::THRESH_TOZERO, tr("To zero (value = value > threshold ? value : 0)"));
    mMode.addChoice(cv::THRESH_TOZERO_INV, tr("To zero inverted (value = value > threshold ? 0 : value)"));
    mMode.addChoice(cv::THRESH_TRUNC, tr("Truncate (value = value > threshold ? threshold : value)"));
    mMode.addChoice(cv::THRESH_OTSU, tr("Otsu’s Algorithm (threshold automatically determined)"));
    mMode.addChoice(cv::THRESH_OTSU+1, tr("Otsu’s Algorithm inverted (threshold automatically determined)"));
    mMode.addChoice(cv::THRESH_TRIANGLE, tr("Triangle algorithm (threshold automatically determined)"));
    mMode.addChoice(cv::THRESH_TRIANGLE+1, tr("Triangle algorithm inverted (threshold automatically determined)"));
    mMode.setDefault(cv::THRESH_BINARY);
    addInputPort(mMode);
}

void ThresholdFilter::execute()
{
    int threshold = mThreshold;
    cv::Mat src = mIn;
    ((Image)src).convertToGray(CV_8U);

    Image dest;
    if(mMode == cv::THRESH_OTSU || mMode == cv::THRESH_TRIANGLE){
        cv::threshold(src, dest, 0, 255, cv::THRESH_BINARY + mMode);
    } else if(mMode == cv::THRESH_OTSU+1){
        cv::threshold(src, dest, 0, 255, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);
    }else if(mMode == cv::THRESH_TRIANGLE+1){
        cv::threshold(src, dest, 0, 255, cv::THRESH_BINARY_INV + cv::THRESH_TRIANGLE);
    } else {
        cv::threshold(src, dest, threshold, 255, mMode);
    }
    mOut.send(dest);
}
