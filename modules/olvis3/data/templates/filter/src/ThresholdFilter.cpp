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

#include "ThresholdFilter.h"
#include <opencv2/imgproc/imgproc.hpp>

REGISTER_FILTER(ThresholdFilter);


ThresholdFilter::ThresholdFilter()
{
    setName("ThresholdFilter");
    setDesc("Thresholds an image");
    setGroup("user");
    mOut.setName("outImage");
    mOut.setDesc("image output port");
    addOutputPort(mOut);
    mIn.setName("inImage");
    mIn.setDesc("image input");
    addInputPort(mIn);
    mThreshold.setName("threshold");
    mThreshold.setDesc("Threshold value");
    mThreshold.setDefault(128);
    mThreshold.setRange(0, 255);
    addInputPort(mThreshold);

    mMode.setName("mode");
    mMode.setDesc("Threshold mode");
    mMode.addChoice(cv::THRESH_BINARY, "Binary");
    mMode.addChoice(cv::THRESH_BINARY_INV, "Binary inverted");
    mMode.addChoice(cv::THRESH_TOZERO, "to zero");
    mMode.setDefault(cv::THRESH_BINARY);
    addInputPort(mMode);
}

void ThresholdFilter::execute()
{
    int threshold= mThreshold;
	int mode = mMode;
    const GrayImage& src = mIn;
    GrayImage dest;
    cv::threshold(src, dest, threshold, 255, mode);
    mOut.send(dest);
}
