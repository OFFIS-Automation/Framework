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

#include "Sharp.h"
#include <opencv2/imgproc.hpp>

REGISTER_FILTER(Sharp);
Sharp::Sharp()
{
    setName("Sharpening");
    setDesc(QObject::tr("Sharpening image using `unsharp mask` algorithm"));
    setGroup("image/sharpening");

    mOut.setName("imageOut");
    mOut.setDesc(QObject::tr("Image output"));
    addOutputPort(mOut);

    mIn.setName("imageIn");
    mIn.setDesc(QObject::tr("Image input"));
    addInputPort(mIn);

    mKernelSize.setName("kernelSize");
    mKernelSize.setDefault(3);
    mKernelSize.setOnlyOdd();
    addInputPort(mKernelSize);

    mIterarions.setName("iteration");
    mIterarions.setDefault(1);
    addInputPort(mIterarions);
}

void Sharp::execute()
{
    const cv::Mat src = mIn;
    int kernelSize = mKernelSize;
    int iterations = mIterarions;

    cv::Mat dest;
    cv::GaussianBlur(src, dest, cv::Size(kernelSize, kernelSize), iterations);
    cv::addWeighted(src, 1.5, dest, -0.5, 0, dest);

    mOut.send(dest);
}
