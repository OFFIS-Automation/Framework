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

#include "PhysicalImageSize.h"

REGISTER_FILTER(PhysicalImageSize);

PhysicalImageSize::PhysicalImageSize()
{
    setName("PhysicalImageSize");
    setDesc("Inverts the image");
    setGroup("measurement");
    mIn.setName("input");
    mOut.setName("output");
    addInputPort(mIn);
    addOutputPort(mOut);
    mMmPerPixel.setName("mm/pixel");
    mMmPerPixel.setIsPysicalPixelSize(true);
    addOutputPort(mMmPerPixel);
    mTotalWidth.setName("width");
    mTotalWidth.setDesc("The total image width im mm");
    addInputPort(mTotalWidth);

}

void PhysicalImageSize::execute()
{
    const cv::Mat input = mIn;
    mOut.send(input);
    double value = mTotalWidth.getValue() / 1000.0 / (double)input.cols;
    mMmPerPixel.send(value);
}
