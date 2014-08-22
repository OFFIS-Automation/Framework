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

#include "FetchTemplate.h"

REGISTER_FILTER(FetchTemplate);

FetchTemplate::FetchTemplate()
{
    setName("FetchTemplate");
    setDesc("Fetches a template from the given image on the ROI position.");
    setGroup("image/reshape");
    mIn.setName("input");
    mOut.setName("output");
    addInputPort(mIn);
    addOutputPort(mOut);

    mRoi.setName("newRoi");
    mRoi.setDesc("Region where the template is extracted. Only applied when changed.");
    mRoi.setMode(SingleShotPortMode);
    mRoi.setDisplayVisibility(false);
    addInputPort(mRoi);

}

void FetchTemplate::execute()
{
    if(mRoi.hasValue())
    {
        QRect roi = mRoi;
        const cv::Mat source = mIn;
        QRect imageRect = QRect(0,0, source.cols, source.rows);
        roi = roi.intersected(imageRect);
        if(roi.isValid())
        {

            mTemplate = source(port::Rect::rect(roi));
        }
    }
    if(mTemplate.data)
        mOut.send(mTemplate);
}
