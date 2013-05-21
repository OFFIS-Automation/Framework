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

#include "ScaleImage.h"
#include <opencv2/imgproc/imgproc.hpp>

REGISTER_FILTER(ScaleImage);
ScaleImage::ScaleImage()
{
    setName("ScaleImage");
    setDesc("Scales an image by a factor.");
    setGroup("image/reshape");
    mScale.setName("scale");
    mScale.setRange(10, 1000);
    mScale.setDefault(100.0);
    addInputPort(mIn);
    addInputPort(mScale);
    addOutputPort(mOut);
}

void ScaleImage::execute()
{
    const cv::Mat in = mIn;
    cv::Mat out;
    double scale = mScale;
    scale /= 100.0;
    cv::resize(in, out, cv::Size(), scale, scale);
    mOut.send(out);
}
