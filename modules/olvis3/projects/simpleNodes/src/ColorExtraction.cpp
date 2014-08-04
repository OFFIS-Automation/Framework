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

#include "ColorExtraction.h"
#include <opencv2/imgproc/imgproc.hpp>

#include <QDebug>

REGISTER_FILTER(ColorExtraction);
ColorExtraction::ColorExtraction()
{
    setName("ColorExtraction");
    setDesc("Extracts a color from an input image. Takes either the color values provided by value or at a point.");
    setGroup("image/color");
    mIn.setName("imageIn");
    addInputPort(mIn);

    mInPoint.setName("input point");
    mInPoint.setMode(OptionalPortMode);
    addInputPort(mInPoint);
    mOutPoint.setName("output point");
    addOutputPort(mOutPoint);

    mRValue.setName("red value (ignored when input point is set)");
    mRValue.setDefault(128);
    mRValue.setRange(0, 255);
    addInputPort(mRValue);
    mGValue.setName("green value (ignored when input point is set)");
    mGValue.setDefault(128);
    mGValue.setRange(0, 255);
    addInputPort(mGValue);
    mBValue.setName("blue value (ignored when input point is set)");
    mBValue.setDefault(128);
    mBValue.setRange(0, 255);
    addInputPort(mBValue);

    mTolerance.setName("tolerance");
    mTolerance.setDefault(0.0);
    mTolerance.setRange(0.0, 100.0);
    addInputPort(mTolerance);

    mDefaultValue.setName("default value");
    mDefaultValue.setDefault(0);
    mDefaultValue.setRange(0, 255);
    addInputPort(mDefaultValue);

    mOut.setName("imageOut");
    addOutputPort(mOut);

    mColorOut.setName("colorOut");
    addOutputPort(mColorOut);
}

void ColorExtraction::execute()
{
    const cv::Mat input = mIn;
    // Select which color value to choose
    int values[3] = {mBValue.getValue(), mGValue.getValue(), mRValue.getValue()};
    if (mInPoint.hasValue()){
        cv::Point2d point = mInPoint.getValue();
        mOutPoint.send(point);

        cv::Vec3b pointValues = input.at<cv::Vec3b>(point.y, point.x);
        values[0] = pointValues.val[0]; // B
        values[1] = pointValues.val[1]; // G
        values[2] = pointValues.val[2]; // R
    }
    float tolerance = mTolerance.getValue();
    int defaultValue = mDefaultValue.getValue();

    cv::Mat color = cv::Mat(1, 1, CV_8UC3);
    color.setTo(cv::Scalar(values[0], values[1], values[2]));
    mColorOut.send(color);

    // Extract color
    std::vector<cv::Mat> channels;
    cv::split(input, channels);

    for(unsigned i=0; i<channels.size(); i++){
        // Tolerance (half lower, half upper)
        int minValue = values[i] * (1-((tolerance / 100.0) / 2));
        int maxValue = values[i] * (1+((tolerance / 100.0) / 2));

        cv::threshold(channels[i], channels[i], minValue, defaultValue, cv::THRESH_TOZERO);
        cv::threshold(channels[i], channels[i], maxValue, defaultValue, cv::THRESH_TOZERO_INV);
    }

    cv::Mat dest;
    cv::merge(channels, dest);
    mOut.send(dest);
}
