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

#include "NormalizeHistogramm.h"
#include <opencv2/imgproc.hpp>

REGISTER_FILTER(NormalizeHistogramm);
NormalizeHistogramm::NormalizeHistogramm()
{
    setGroup("image/analysis");
    setName("EqualizeHistogram");
    setDesc(QObject::tr("Improves the contrast of an image by equalizing the histogram"));

    mIn.setName("imageIn");
    mIn.setDesc(QObject::tr("Image input"));
    addInputPort(mIn);

    mOut.setName("imageOut");
    mOut.setDesc(QObject::tr("Image output"));
    addOutputPort(mOut);
}

void NormalizeHistogramm::execute()
{
    cv::Mat src = mIn;
    ((Image)src).convertToGray(CV_8U);

    cv::Mat dest;
    cv::equalizeHist(src, dest);
    mOut.send(dest);
}
