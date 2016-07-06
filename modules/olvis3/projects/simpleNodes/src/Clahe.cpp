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

#include "Clahe.h"
#include <opencv2/imgproc.hpp>

REGISTER_FILTER(Clahe);

Clahe::Clahe()
{
    setName("Contrast Limited Adaptive Histogram Equalization (CLAHE)");
    setGroup("image/analysis");

    mIn.setName("imageIn");
    mIn.setDesc(QObject::tr("Image input"));
    addInputPort(mIn);

    mOut.setName("imageOut");
    mOut.setDesc(QObject::tr("Image output"));
    addOutputPort(mOut);

    mClipLimit.setName("clipLimit");
    mClipLimit.setDefault(4);
    addInputPort(mClipLimit);
}

void Clahe::execute()
{
    const cv::Mat input = mIn;
    int clipLimit = mClipLimit;

    // Convert image to LAB color space
    cv::Mat labImage;
    cv::cvtColor(input, labImage, CV_BGR2Lab);

    // Extract the L channel
    std::vector<cv::Mat> lapPlanes(3);
    cv::split(labImage, lapPlanes);  // now we have the L image in lab_planes[0]

    // Apply the CLAHE algorithm to the L channel
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(clipLimit);
    cv::Mat dst;
    clahe->apply(lapPlanes[0], dst);

    // Merge the the color planes back into an Lab image
    dst.copyTo(lapPlanes[0]);
    cv::merge(lapPlanes, labImage);

    // Convert back to RGB color space
    cv::Mat claheImage;
    cv::cvtColor(labImage, claheImage, CV_Lab2BGR);

    mOut.send(claheImage);
}
