// OFFIS Automation Framework
// Copyright (C) 2013-2018 OFFIS e.V.
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

#include "BrighnessContrast.h"
#include <opencv2/imgproc.hpp>

REGISTER_FILTER(BrighnessContrast);
BrighnessContrast::BrighnessContrast()
{
    setName("BrightnessContrast");
    setDesc(QObject::tr("Adjusts brightness and contrast"));
    setGroup("image/color");

    mIn.setName("imageIn");
    mIn.setDesc(QObject::tr("Image input"));
    addInputPort(mIn);

    mOut.setName("imageOut");
    mOut.setDesc(QObject::tr("Image output"));
    addOutputPort(mOut);

    mBrightness .setName("brightnessFactor");
    mBrightness .setDesc(QObject::tr("Adjust the brightness"));
    mBrightness .setDefault(0);
    mBrightness .setRange(-100, 100);
    addInputPort(mBrightness );

    mContrast.setName("contrastFactor");
    mContrast.setDesc(QObject::tr("Adjust the contrast"));
    mContrast.setDefault(0);
    mContrast.setRange(-100, 100);
    addInputPort(mContrast);
}

void BrighnessContrast::execute()
{
    double brightness = mBrightness .getValue();
    double contrast = mContrast.getValue();
    contrast /= 100;
    brightness *=65535/100;

    cv::Mat src = mIn;
    cv::Mat workingImage;
    src.convertTo(workingImage, CV_16U);

    cv::Mat dest;
    workingImage.convertTo(dest, -1, 1.0 + contrast, brightness);

    mOut.send(dest);
}
