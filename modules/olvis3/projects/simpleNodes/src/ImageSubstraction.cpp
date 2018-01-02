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

#include "ImageSubstraction.h"
#include <opencv2/imgproc/imgproc.hpp>

REGISTER_FILTER(ImageSubstraction);
ImageSubstraction::ImageSubstraction()
{
    setName("ImageSubstraction");
    setDesc(QObject::tr("Subtracts a background from an input image"));
    setGroup("image/math");

    mIn.setName("imageIn");
    mIn.setDesc(QObject::tr("Image input"));
    addInputPort(mIn);

    mOut.setName("imageOut");
    mOut.setDesc(QObject::tr("Image output"));
    addOutputPort(mOut);

    mBackgroundIn.setName("backgroundIn");
    addInputPort(mBackgroundIn);
}

void ImageSubstraction::execute()
{
    const cv::Mat input = mIn;
    const cv::Mat background = mBackgroundIn;

    cv::Mat dest = input - background;

    mOut.send(dest);
}
