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

#include "ColorExtraction.h"
#include <opencv2/imgproc/imgproc.hpp>

#include <QDebug>

REGISTER_FILTER(ColorExtraction);
ColorExtraction::ColorExtraction()
{
    setName("ColorExtraction");
    setDesc("Extracts a color from an input image. The result image contains "
            "all pixels that are close to the given color within a given "
            "tolerance");
    setGroup("image/color");
    mIn.setName("imageIn");
    mIn.setDesc("input image");
    addInputPort(mIn);

    mColor.setName("color");
    mColor.setDesc("The color to search for");
    addInputPort(mColor);

    mTolerance.setName("tolerance");
    mTolerance.setDesc("The search tolerance in pixel. a value of 10 results "
                       "in a search window of [c-5:c+5]");
    mTolerance.setOnlyEven(false);
    mTolerance.setDefault(10);
    mTolerance.setRange(0, 128);
    addInputPort(mTolerance);

    mFillColor.setName("fillColor");
    mFillColor.setDesc(
        "The color for the pixels that are not within the found range");
    mFillColor.setVisibility(AdvancedPortVisibility);
    mFillColor.setMode(OptionalPortMode);
    addInputPort(mFillColor);

    mOut.setName("imageOut");
    mOut.setDesc("output image");
    addOutputPort(mOut);
}

void ColorExtraction::execute()
{
    const RgbImage input = mIn;

    cv::Vec3b color = mColor.bgr();
    int tolerance = mTolerance.getValue();

    // calculate the lower and upper border for the allowed colors
    cv::Scalar low(color[0] - tolerance / 2, color[1] - tolerance / 2,
                   color[2] - tolerance / 2);
    cv::Scalar high(color[0] + tolerance / 2, color[1] + tolerance / 2,
                    color[2] + tolerance / 2);

    // create a mask where all pixels that are between the borders are white
    // (255) and all others
    // are black (0)
    cv::Mat mask;
    cv::inRange(input, low, high, mask);

    // convert to 3 channel matrix
    std::vector<cv::Mat> channels;
    channels.push_back(mask);
    channels.push_back(mask);
    channels.push_back(mask);
    cv::merge(channels, mask);

    // only take the pixel values from original image where the mask is 255
    cv::Mat dest;
    cv::Mat foreground;
    cv::bitwise_and(input, mask, foreground);
    if(mFillColor.hasValue())
    {
        cv::Vec3b backgroundColor = mFillColor.bgr();
        // for the background, invert the mask
        bitwise_not(mask, mask);
        cv::Mat background;
        cv::bitwise_and(backgroundColor, mask, background);

        // mix the channels together. can use bitwise_or or max, don't know which is faster
        cv::bitwise_or(foreground, background, dest);
    }
    else
        dest = foreground;
    mOut.send(dest);
}
