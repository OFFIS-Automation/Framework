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

#include "WhiteBalance.h"
#include <opencv2/opencv.hpp>
REGISTER_FILTER(WhiteBalance);

WhiteBalance::WhiteBalance()
{
    setName("WhiteBalance");
    setDesc(QObject::tr("Simple white balance based on grayworld white balance algorithm"));
    setGroup("image/color");

    mIn.setName("imageIn");
    mIn.setDesc(QObject::tr("Image input"));
    addInputPort(mIn);

    mOut.setName("imageOut");
    mOut.setDesc(QObject::tr("Image output"));
    addOutputPort(mOut);
}

void WhiteBalance::execute()
{
    const cv::Mat src = mIn;
    cv::Mat dest;
    if(src.rows > 0 && src.cols > 0){
        // http://opencvintro.blogspot.de/2015/05/grey-world-algorithm-in-opencv.html
        cv::Scalar sumImg = cv::sum(src);
        cv::Scalar illum = sumImg/(src.rows*src.cols);

        std::vector<cv::Mat> rgbChannels(3);
        cv::split(src, rgbChannels);

        cv::Mat redImg = rgbChannels[2];
        cv::Mat greenImg = rgbChannels[1];
        cv::Mat blueImg = rgbChannels[0];

        // Calculate scale factor for normalisation you can use 255 instead
        double scale=(illum(0)+illum(1)+illum(2))/3;

        // Correct for illuminant (white balancing)
        redImg = redImg*scale/illum(2);
        greenImg = greenImg*scale/illum(1);
        blueImg = blueImg*scale/illum(0);

        merge(rgbChannels, dest);
    }
    mOut.send(dest);
}
