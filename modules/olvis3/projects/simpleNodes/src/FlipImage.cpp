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

#include "FlipImage.h"

REGISTER_FILTER(FlipImage);

FlipImage::FlipImage()
{
    setName("Flip");
    setDesc(QObject::tr("Flips an image horizontal, vertical or both"));
    setGroup("image/reshape");
    addInputPort(mImageIn);
    addOutputPort(mImageOut);
    mFlipModeIn.setName("flipMode");
    mFlipModeIn.addChoice(0, tr("Vertical"));
    mFlipModeIn.addChoice(1, tr("Horizontal"));
    mFlipModeIn.addChoice(-1, tr("Both"));
    mFlipModeIn.setDefault(0);
    addInputPort(mFlipModeIn);
}

void FlipImage::execute()
{
    cv::Mat target;
    const cv::Mat source = mImageIn;
    cv::flip(source, target, (int)mFlipModeIn);
    mImageOut.send(target);
}

