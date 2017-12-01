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

#include "Inpainting.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>

REGISTER_FILTER(Inpainting);
Inpainting::Inpainting()
{
    setName("Inpainting");
    setDesc(QObject::tr(""));
    setGroup("image/test");

    mIn.setName("imageIn");
    mIn.setDesc(QObject::tr("Image input"));
    addInputPort(mIn);

    mRadius.setName("radius");
    mRadius.setDesc(QObject::tr("Radius of a circular neighborhood of each point inpainted"));
    mRadius.setRange(1, 20);
    mRadius.setDefault(3);
    addInputPort(mRadius);

    mInMask.setName("mask");
    mInMask.setDesc(QObject::tr("Mask input"));
    addInputPort(mInMask);

    mOut.setName("imageOut");
    mOut.setDesc(QObject::tr("Image output"));
    addOutputPort(mOut);

    mMode.setName("mode");
    mMode.setDesc(QObject::tr("Select the inpainting mode"));
    mMode.addChoice(NS, "NS");
    mMode.addChoice(Telea, "Telea");
    mMode.setDefault(NS);
    addInputPort(mMode);
}

void Inpainting::execute()
{
    Image src = mIn;
    src.convertToGray(CV_8U);
    Image mask = mInMask;
    mask.convertToGray(CV_8U);

    cv::Mat dest;
    cv::inpaint(src, mask, dest, mRadius, mMode);

    mOut.send(dest);
}
