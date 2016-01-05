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

#include "Denoising.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/photo/photo.hpp>

REGISTER_FILTER(Denoising);

Denoising::Denoising()
{
    setName("Non-local Means Denoising");
    setDesc("Removes noise from a given image. (Noise is expected to be gaussian).");
    setGroup("image/smoothing");

    mOut.setName("imageout");
    mOut.setDesc("Image output port");
    addOutputPort(mOut);

    mIn.setName("imageIn");
    mIn.setDesc("Image input port");
    addInputPort(mIn);

    mTemplateWindowSize.setName("templateWindowSize");
    mTemplateWindowSize.setDesc("Size in pixels of the template patch that is used to compute weights.");
    mTemplateWindowSize.setDefault(7);
    mTemplateWindowSize.setOnlyOdd();
    addInputPort(mTemplateWindowSize);

    mSearchWindowSize.setName("searchWindowSize");
    mSearchWindowSize.setDesc("Size in pixels of the window that is used to compute weighted average for given pixel.");
    mSearchWindowSize.setDefault(21);
    mSearchWindowSize.setOnlyOdd();
    addInputPort(mSearchWindowSize);

    mH.setName("h");
    mH.setDesc("Parameter regulating filter strength.");
    mH.setDefault(3);
    addInputPort(mH);
}

void Denoising::execute()
{
    int kTemplateWindowSize = mTemplateWindowSize;
    int kSearchWindowSize = mSearchWindowSize;
    int kH = mH;

    const GrayImage src = mIn;
    GrayImage dest;
    cv::fastNlMeansDenoising(src, dest, kH, kTemplateWindowSize, kSearchWindowSize);
    mOut.send(dest);
}
