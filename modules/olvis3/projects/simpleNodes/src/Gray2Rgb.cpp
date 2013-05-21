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

#include "Gray2Rgb.h"
#include <opencv2/imgproc/imgproc.hpp>

REGISTER_FILTER(Gray2Rgb);

Gray2Rgb::Gray2Rgb()
{
    setName("GrayToRgb");
    setDesc("Converts a gray image to RGB using different filters.");
    setGroup("image/color");
    mMode.setName("mode");
    mMode.setDesc("Conversion mode");
    mMode.addChoice(DebayerRG, "De-bayer starting with rg");
    mMode.addChoice(DebayerGR, "De-bayer starting with gr");
    mMode.addChoice(DebayerBG, "De-bayer starting with bg");
    mMode.addChoice(DebayerGB, "De-bayer starting with gb");
    mMode.setDefault(DebayerRG);
    mMode.setIcon(QImage(":/SimpleNodes/colorMode.png"));
    addInputPort(mMode);
    mIn.setName("imageIn");
    addInputPort(mIn);
    mOut.setName("imageOut");
    addOutputPort(mOut);
}

void Gray2Rgb::execute()
{
    const cv::Mat input = mIn;
    cv::Mat dest;
    int m = mMode;
    switch ((Mode) m) {
    case DebayerRG:
        cv::cvtColor(input, dest, CV_BayerRG2RGB);
        break;
    case DebayerGR:
        cv::cvtColor(input, dest, CV_BayerGR2RGB);
        break;
    case DebayerBG:
        cv::cvtColor(input, dest, CV_BayerBG2RGB);
        break;
    case DebayerGB:
        cv::cvtColor(input, dest, CV_BayerGB2RGB);
        break;
    }
    mOut.send(dest);
}
