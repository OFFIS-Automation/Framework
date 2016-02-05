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


#include "VignetteCreator.h"

//#include <opencv2/imgproc/imgproc.hpp>
REGISTER_FILTER(VignetteCreator);

VignetteCreator::VignetteCreator()
{
    setName("VignetteCreator");
    setDesc(QObject::tr("Creates a Vignette from the provided image. It is assumed, that a RGB image of a clean single colored substrate is provided"));
    setGroup("image/math");

    // define image input port
    mIn.setName("imageIn");
    mIn.setDesc(QObject::tr("Image of clean single colored substrate"));
    addInputPort(mIn);

    // define mode
    mMode.setName("mode");
    mMode.addChoice(Difference, "Difference: I_{channel} - I_min_{channel}");
    mMode.addChoice(Ratio, "Ratio: (I_max_{channel} / I_{channel}) * 255");
    mMode.setDefault(Difference);
    addInputPort(mMode);

    mStringSetPoint.setName("setPoint");
    addInputPort(mStringSetPoint);

    // define output port
    mOut.setName("vignette");
    mOut.setDesc(QObject::tr("Calculated Vignette"));
    addOutputPort(mOut);

    mStringOffset.setName("offset");
    mStringOffset.setDesc(QObject::tr("Comma separated list of the offset to be used (R,G,B)"));
    addOutputPort(mStringOffset);
}

std::vector<double> VignetteCreator::splitString(std::string str)
{
    std::vector<double> values;
    size_t pos = 0;

    while ((pos = str.find(',', 1)) != std::string::npos)
    {
        std::string p = str.substr(0, pos);
        values.push_back(stod(p));
        str = str.substr(pos + 1);
    }

    if (!str.empty()) {
        values.push_back(stod(str));
    }
    return (values);
}

void VignetteCreator::execute()
{
    // do nothing if all input variables are the same
    // prevents unnecessary calculations
    if (!mMode.hasChanged() && !mIn.hasChanged() &&  !mStringSetPoint.hasChanged())
        return;

    if (mStringSetPoint.hasChanged())
        mSetPointVector = splitString(mStringSetPoint.getValue().toStdString());

    const cv::Mat input = mIn;
    const int mode = mMode;

    // return variable
    cv::Mat dest = input.clone();

    // spliting into single channels
    std::vector<cv::Mat> channels(3);
    cv::split(input,channels);

    float min[3];

    if (mode == Difference)
    {
        // calculate minimum / maximum for the single channels
        double minValue;
        for (int i=0; i <= 2; i++) {
            // calculate minimum value
            cv::minMaxIdx(channels[i], &minValue, NULL, NULL, NULL);
            min[i] = minValue;

            // substract minimum value
            cv::subtract(channels[i],minValue, channels[i]);
        }
        // remerge the channels
        cv::merge(channels, mVignette);
    }
    else if (mode == Ratio)
    {
        // calculate minimum / maximum for the single channels

        double minValue;
        double maxValue;
        for (int i=0; i <= 2; i++) {
            // convert to double
            channels[i].convertTo(channels[i], CV_64FC3);

            // calculate minimum value
            cv::minMaxLoc(channels[i], &minValue, &maxValue, NULL, NULL);
            min[i] = minValue;

            // calculate factors
            channels[i] = maxValue / channels[i];

            // calculate minimum value
            cv::minMaxLoc(channels[i], NULL, &maxValue, NULL, NULL);

            channels[i] /= maxValue;
            channels[i] *= 255;

        }
        // remerge the channels
        cv::merge(channels, mVignette);
    }
    // output minimal values of image
    mOffsetString = "";

    for (int i=0; i <= 2; i++) {
        long double offset = mSetPointVector.at(i) - min[2-i];

        mOffsetString += std::to_string(offset);

        if (i != 2) {
            mOffsetString += ",";
        }
    }

    // output image
    mOut.send(mVignette);
    mStringOffset.send(QString::fromStdString(mOffsetString));
}
