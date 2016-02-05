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


#include "BrighnessContrast.h"
#include <opencv2/imgproc/imgproc.hpp>

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

    mBrighness.setName("brightnessFactor");
    mBrighness.setDesc(QObject::tr("Adjust the brightness"));
    mBrighness.setDefault(0);
    mBrighness.setRange(-100, 100);
    addInputPort(mBrighness);

    mContrast.setName("contrastFactor");
    mContrast.setDesc(QObject::tr("Adjust the contrast"));
    mContrast.setDefault(0);
    mContrast.setRange(-100, 100);
    addInputPort(mContrast);
}

void BrighnessContrast::execute()
{

    double brighness = mBrighness.getValue();
    double contrast = mContrast.getValue();
    contrast /= 100;
    brighness *=255/100;
    cv::Mat source = mIn;
    cv::Mat wide;
    source.convertTo(wide, CV_16S, 1.0, -127);
    if(source.channels() == 4)
    {
        RgbImage rgb(source);
        source = rgb;
    }
    cv::Mat target;
    wide.convertTo(target,-1,1.0 + contrast, brighness + 127);
    mOut.send(target);
}
