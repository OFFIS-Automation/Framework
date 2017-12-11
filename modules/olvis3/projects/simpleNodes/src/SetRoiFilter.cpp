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

#include "SetRoiFilter.h"

REGISTER_FILTER(SetRoiFilter);

SetRoiFilter::SetRoiFilter()
{
    setName("ImageRoi");
    setDesc(QObject::tr("Extracts a region of interest from an image"));
    setGroup("image/reshape");

    mIn.setName("imageIn");
    mIn.setDesc(QObject::tr("image input"));
    addInputPort(mIn);

    mOut.setName("imageOut");
    mOut.setDesc(QObject::tr("image output"));
    addOutputPort(mOut);

    mOffsetIn.setName("offsetIn");
    mOffsetIn.setDesc(QObject::tr("adjust the position of the ROI using this input port"));
    mOffsetIn.setMode(OptionalPortMode);
    mOffsetIn.setVisibility(AdvancedPortVisibility);
    addInputPort(mOffsetIn);

    mOffsetModeIn.setName("offsetInMode");
    mOffsetModeIn.setDesc(QObject::tr("Where to apply the input offset"));
    mOffsetModeIn.addChoice(TopLeftOffset, "Top left");
    mOffsetModeIn.addChoice(CenterOffset, "center");
    mOffsetModeIn.setVisibility(AdvancedPortVisibility);
    mOffsetModeIn.setDefault(TopLeftOffset);
    addInputPort(mOffsetModeIn);

    mRoiIn.setName("roi");
    mRoiIn.setDesc(QObject::tr("The selected region of interest x"));
    mRoiIn.setMode(OptionalPortMode);
    mRoiIn.setDisplayVisibility(false);
    addInputPort(mRoiIn);

    mOffsetOut.setName("imageOffset");
    mOffsetOut.setDesc(QObject::tr("Image top left offset point"));
    addOutputPort(mOffsetOut);
}

void SetRoiFilter::execute()
{
    const cv::Mat src = mIn;
    cv::Mat dest = src.clone();
    cv::Point offset(0, 0);
    if (mRoiIn.hasValue()) {
        cv::Rect rect = mRoiIn;
        if (mOffsetIn.hasValue()) {
            cv::Point offsetIn = mOffsetIn.getValue();
            int mode = mOffsetModeIn.getValue();
            if (mode == TopLeftOffset) {
                rect.x = offsetIn.x;
                rect.y = offsetIn.y;
            } else if (mode == CenterOffset) {
                rect.x = offsetIn.x - rect.width / 2;
                rect.y = offsetIn.y - rect.height / 2;
            }
        }
        dest = cv::Mat(src, rect).clone();
        offset = rect.tl();
    }
    mOut.send(dest);
    mOffsetOut.send(offset);
}
