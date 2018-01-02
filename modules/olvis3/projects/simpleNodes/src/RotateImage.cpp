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

#include "RotateImage.h"
#include <opencv2/imgproc.hpp>

REGISTER_FILTER(RotateImage);
RotateImage::RotateImage()
{
    setName("Rotate");
    setGroup("image/reshape");
    setDesc(QObject::tr("Rotates an image"));

    mOut.setName("imageOut");
    mOut.setDesc(QObject::tr("Image output"));
    addOutputPort(mOut);

    mIn.setName("imageIn");
    mIn.setDesc(QObject::tr("Image input"));
    addInputPort(mIn);

    mAngle.setName("angle");
    mAngle.setDesc(QObject::tr("Angle of the rotation"));
    mAngle.addChoice(0, tr("None"));
    mAngle.addChoice(90, tr("90 degree clockwise"));
    mAngle.addChoice(180, tr("180 degree"));
    mAngle.addChoice(270, tr("90 degree counter-clockwise"));
    mAngle.setDefault(0);
    addInputPort(mAngle);
}

void RotateImage::execute()
{
    const cv::Mat src = mIn;
    int angle = mAngle;
    cv::Mat dest;
    if(angle == 90)
    {
        dest = src.t();
        cv::flip(dest, dest, 1);
    }
    else if(angle == 180)
    {
        cv::flip(src, dest, -1);
    }
    else if(angle == 270)
    {
        dest = src.t();
        cv::flip(dest, dest, 0);
    }
    else
    {
        dest = src;
    }

    mOut.send(dest);
}
