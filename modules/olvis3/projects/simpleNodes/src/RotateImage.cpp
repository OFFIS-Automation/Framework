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

#include "RotateImage.h"

#include <opencv2/imgproc/imgproc.hpp>
REGISTER_FILTER(RotateImage);

RotateImage::RotateImage()
{
    setName("RotateImage");
    setGroup("image/reshape");
    setDesc("Rotates an image.");
    mAngle.setName("angle");
    mAngle.setDesc("Angle of the rotation");
    mAngle.addChoice(0, "None");
    mAngle.addChoice(90, "90° clockwise");
    mAngle.addChoice(180, "180°");
    mAngle.addChoice(270, "90° counterclockwise");
    mAngle.setDefault(0);
    addInputPort(mAngle);
    addInputPort(mIn);
    addOutputPort(mOut);

}

void RotateImage::execute()
{
    const cv::Mat source = mIn;
    int angle = mAngle;
    cv::Mat dest;
    if(angle == 90)
    {
        dest = source.t();
        cv::flip(dest, dest, 1);
    }
    else if(angle == 180)
    {
        cv::flip(source, dest, -1);
    }
    else if(angle == 270)
    {
        dest = source.t();
        cv::flip(dest, dest, 0);
    }
    else
    {
        dest = source;
    }

    mOut.send(dest);
}
