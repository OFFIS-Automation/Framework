// OFFIS Automation Framework
// Copyright (C) 2013-2014 OFFIS e.V.
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

#include "SetRoiFilter.h"

REGISTER_FILTER(SetRoiFilter);

SetRoiFilter::SetRoiFilter()
{
    setName("ImageRoi");
    setDesc("Extracts a region of interest from an image.");
    setGroup("image/reshape");
    mImageOut.setName("imageIn");
    mImageIn.setDesc("image input");
    addInputPort(mImageIn);
    mRoiIn.setName("roi");
    mRoiIn.setDesc("The selected region of interest");
    mRoiIn.setMode(OptionalPortMode);
    mRoiIn.setDisplayVisibility(false);
    addInputPort(mRoiIn);
    mImageOut.setName("imageOut");
    mImageOut.setDesc("image output");
    addOutputPort(mImageOut);
    mOffsetOut.setName("imageOffset");
    mOffsetOut.setDesc("Image top left offset point");
    addOutputPort(mOffsetOut);
}

void SetRoiFilter::execute()
{
    const cv::Mat input = mImageIn;
    cv::Mat img = input;
    cv::Point offset(0, 0);
    if(mRoiIn.hasValue())
    {
        cv::Rect rect = mRoiIn;
        img = cv::Mat(img, rect);
        offset = rect.tl();
    }
    mImageOut.send(img);
    mOffsetOut.send(offset);
}
