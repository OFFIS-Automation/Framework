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

#include "BilateralFilter.h"
#include <opencv2/imgproc.hpp>

REGISTER_FILTER(Bilateral);

Bilateral::Bilateral()
{
    setName("Bilateral");
    setDesc(QObject::tr("Removes noise from a given image by applying the bilateral filter to an image"));
    setGroup("image/smoothing");

    mIn.setName("imageIn");
    mIn.setDesc(QObject::tr("Image input"));
    addInputPort(mIn);

    mOut.setName("imageOut");
    mOut.setDesc(QObject::tr("Image output"));
    addOutputPort(mOut);

    mD.setName("d");
    mD.setDesc(QObject::tr("Diameter of each pixel neighborhood that is used during filtering"));
    mD.setDefault(3);
    mD.setOnlyOdd();
    mD.setRange(3, 100);
    addInputPort(mD);

    mSigmaColor.setName("sigmaColor");
    mSigmaColor.setDesc(QObject::tr("Filter sigma in the color space. A larger value of the parameter means that farther colors within the pixel neighborhood (see sigmaSpace ) will be mixed together, resulting in larger areas of semi-equal color"));
    addInputPort(mSigmaColor);

    mSigmaSpace.setName("sigmaSpace");
    mSigmaSpace.setDesc(QObject::tr("Filter sigma in the coordinate space. A larger value of the parameter means that farther pixels will influence each other as long as their colors are close enough"));
    addInputPort(mSigmaSpace);
}

void Bilateral::execute()
{
    int kD = mD;
    int kSigmaColor = mSigmaColor;
    int kSigmaSpace = mSigmaSpace;

    const cv::Mat src = mIn;
    cv::Mat dest;

    cv::bilateralFilter(src, dest, kD, kSigmaColor, kSigmaSpace);

    mOut.send(dest);
}
