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

#include "ColorPicker.h"
#include <opencv2/imgproc/imgproc.hpp>

REGISTER_FILTER(ColorPicker);

ColorPicker::ColorPicker()
{
    setName("ColorPicker");
    setDesc("extracs the color of a given point of the input image");
    setGroup("image/color");
    mIn.setName("input");
    mIn.setDesc("input image");
    addInputPort(mIn);
    mPointIn.setName("position");
    mPointIn.setDesc("The position inside the image where the color is extracted");
    mPointIn.setDefault(QPointF(0,0));
    addInputPort(mPointIn);
    mColorOut.setName("color");
    mColorOut.setDesc("The extracted color");
    addOutputPort(mColorOut);

}

void ColorPicker::execute()
{
    cv::Mat source = mIn;
    cv::Vec4b bgra(0,0,0,0xFF);
    cv::Point2d p = mPointIn.getValue();
    if(source.channels() == 1)
    {
        uchar gray = source.at<uchar>(p.y, p.x);
        bgra[0] = gray;
        bgra[1] = gray;
        bgra[2] = gray;
    } else if(source.channels() == 3)
    {
        cv::Vec3b bgr = source.at<cv::Vec3b>(p.y, p.x);
        bgra[0] = bgr[0];
        bgra[1] = bgr[1];
        bgra[2] = bgr[2];
    } else if(source.channels() == 4) {
        bgra = source.at<cv::Vec4b>(p.y, p.x);
    }
    mColorOut.sendBgra(bgra);
}
