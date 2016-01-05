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

#include "VignettingCorrectionRGB.h"
#include <opencv2/imgproc/imgproc.hpp>
REGISTER_FILTER(VignettingCorrectionRGB);

VignettingCorrectionRGB::VignettingCorrectionRGB()
{
    setName("VignettingCorrectionRGB");
    setDesc("Corrects vignetting, a common lens effect, uisng a given vignette-mask.");
    setGroup("image/math");

    mIn.setName("imageIn");
    mIn.setDesc("Image or stream to correct.");
    addInputPort(mIn);

    mVignetteIn.setName("VignetteIn");
    mVignetteIn.setDesc("Vignette to correct the imageIn.");
    addInputPort(mVignetteIn);

    mMode.setName("mode");
    mMode.addChoice(Difference, "Difference: I_{channel} - V_{channel}");
    mMode.addChoice(Ratio, "Ratio: I_{channel} * (V_{channel}/ V_max_{channel}})");
    mMode.setDefault(Difference);
    addInputPort(mMode);

    mStringOffset.setName("rgb offset");
    mStringOffset.setDesc("Comma seperated int values that will be added to the result image. This way the output can be normed.");
    mStringOffset.setDefault("0,0,0");
    addInputPort(mStringOffset);

    mOut.setName("output");
    mOut.setDesc("Corrected output for imageIn");
    addOutputPort(mOut);
}

std::vector<double> VignettingCorrectionRGB::splitString(std::string str)
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

void VignettingCorrectionRGB::execute()
{
    cv::Mat dest;
    int mode = mMode;

    if (mStringOffset.hasChanged())
        mOffsetVector = splitString(mStringOffset.getValue().toStdString());

    // spliting into single channels
    std::vector<cv::Mat> image(3);
    std::vector<cv::Mat> vignette(3);
    cv::split(mIn,image);
    cv::split(mVignetteIn,vignette);

    if (mode == Difference)
    {
        for (int i = 0; i <= 2; i++)
        {
            cv::subtract(image[i], vignette[i], image[i]);
        }
    }
    else if (mode == Ratio)
    {
        for (int i = 0; i <= 2; i++)
        {
            // Convert to float values
            cv::Mat vignetteFloat;
            cv::Mat inputFloat;

            vignette[i].convertTo(vignetteFloat, CV_32FC1);
            image[i].convertTo(inputFloat, CV_32FC1);

            // Get max value
            double maxVal;
            cv::minMaxLoc(vignetteFloat, NULL, &maxVal);

            cv::Mat normalized = vignetteFloat / maxVal;
            image[i] = inputFloat.mul(normalized);
        }
    }

    // add offset
    for (int i = 0; i <= 2; i++)
        cv::add(image[i], mOffsetVector.at(2-i), image[i]);

    cv::merge(image, dest);
    mOut.send(dest);
}
