// OFFIS Automation Framework
// Copyright (C) 2014 OFFIS e.V.
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

#include "VignettingCorrection.h"
#include <opencv2/imgproc/imgproc.hpp>

REGISTER_FILTER(VignettingCorrection);
VignettingCorrection::VignettingCorrection()
{
    setName("VignettingCorrection");
    setDesc("Corrects vignetting, a common lens effect.");
    setGroup("image/convert");
    mIn.setName("imageIn");
    addInputPort(mIn);

    mVignetteIn.setName("vignetteIn");
    addInputPort(mVignetteIn);

    mOut.setName("imageOut");
    addOutputPort(mOut);
}

void VignettingCorrection::execute()
{
    // Convert to float values
    const cv::Mat vignette = mVignetteIn;
    cv::Mat vignetteFloat;
    vignette.convertTo(vignetteFloat, CV_32FC1);

    const cv::Mat input = mIn;
    cv::Mat inputFloat;
    input.convertTo(inputFloat, CV_32FC1);

    // Get max value
    double maxVal;
    cv::minMaxLoc(vignetteFloat, NULL, &maxVal);
    cv::Mat normalized = vignetteFloat / maxVal;
    cv::Mat dest = inputFloat.mul(normalized);

    mOut.send(dest);
}
