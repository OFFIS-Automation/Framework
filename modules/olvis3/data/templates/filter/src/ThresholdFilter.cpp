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
    setDesc("Thresholds an image");
    setGroup("user");

    mOut.setName("imageOut");
    addOutputPort(mOut);

    mIn.setName("imageIn");
    addInputPort(mIn);

    mThreshold.setName("threshold");
    mThreshold.setDesc("Threshold value");
    mThreshold.setDefault(128);
    mThreshold.setRange(0, 255);
    addInputPort(mThreshold);

    mMode.setName("mode");
    mMode.setDesc("Thresholding mode");
    mMode.addChoice(cv::THRESH_BINARY, "Binary (value = value > threshold ? max_value : 0 )");
    mMode.addChoice(cv::THRESH_BINARY_INV, "Binary inverted (value = value > threshold ? 0 : max_value)");
    mMode.addChoice(cv::THRESH_TOZERO, "To zero (value = value > threshold ? value : 0)");
    mMode.addChoice(cv::THRESH_TOZERO_INV, "To zero inverted (value = value > threshold ? 0 : value)");
    mMode.addChoice(cv::THRESH_TRUNC, "Truncate (value = value > threshold ? threshold : value)");
    mMode.setDefault(cv::THRESH_BINARY);
    addInputPort(mMode);
}

void ThresholdFilter::execute()
{
    int threshold = mThreshold;
    const GrayImage src = mIn;
    GrayImage dest;
    cv::threshold(src, dest, threshold, 255, mMode);
    mOut.send(dest);
}
