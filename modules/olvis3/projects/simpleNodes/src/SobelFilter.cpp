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

#include "SobelFilter.h"
#include <opencv2/imgproc/imgproc.hpp>

REGISTER_FILTER(SobelFilter);

SobelFilter::SobelFilter()
{
    setName("Sobel");
    setDesc("Applies a sobel filter to an image.");
    setGroup("image/edge detection");
    mIn.setName("imageIn");
    mOut.setName("imageOut");
    addInputPort(mIn);
    addOutputPort(mOut);
}

void SobelFilter::execute()
{
    const cv::Mat source = mIn;
    cv::Mat dest, gradX, gradY, absGradX, absGradY;
    cv::Sobel(source, gradX, CV_16S, 1, 0);
    cv::convertScaleAbs(gradX, absGradX);
    cv::Sobel(source, gradY, CV_16S, 0, 1);
    cv::convertScaleAbs(gradY, absGradY);
    cv::addWeighted(absGradX, 0.5, absGradY, 0.5, 0, dest);
    mOut.send(dest);
}
