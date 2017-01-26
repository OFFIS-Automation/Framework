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

#include "Morphology.h"
#include <opencv2/imgproc.hpp>

REGISTER_FILTER(Morphology);

Morphology::Morphology()
{
    setName("Morphology");
    setGroup("image/math");

    mOperation.setName("operation");
    mOperation.setDesc(QObject::tr("Perform the selected morphology operation"));
    mOperation.addChoice(cv::MORPH_ERODE, tr("erode"));
    mOperation.addChoice(cv::MORPH_DILATE, tr("dilate"));
    mOperation.addChoice(cv::MORPH_OPEN, tr("open"));
    mOperation.addChoice(cv::MORPH_CLOSE, tr("close"));
    mOperation.addChoice(cv::MORPH_GRADIENT, tr("gradient"));
    mOperation.addChoice(cv::MORPH_TOPHAT, tr("top hat"));
    mOperation.addChoice(cv::MORPH_BLACKHAT, tr("black hat"));
    addInputPort(mOperation);

    mIterations.setName("iterations");
    mIterations.setDesc(QObject::tr("Number of iterations to perform"));
    mIterations.setDefault(1);
    mIterations.setRange(1, 255);
    addInputPort(mIterations);

    mElementType.setName("element");
    mElementType.setDesc(QObject::tr("Element type"));
    mElementType.setDefault(cv::MORPH_RECT);
    mElementType.addChoice(cv::MORPH_RECT, tr("Rectangle"));
    mElementType.addChoice(cv::MORPH_ELLIPSE, tr("Ellipse"));
    mElementType.addChoice(cv::MORPH_CROSS, tr("Cross"));
    addInputPort(mElementType);

    mUserElement.setName("specialElement");
    mUserElement.setDesc(QObject::tr("Special structuring element, overrides element and size if connected"));
    mUserElement.setMode(OptionalPortMode);
    mUserElement.setVisibility(AdvancedPortVisibility);
    addInputPort(mUserElement);
}

void Morphology::execute()
{
    int shape = mElementType;
    int size = mKSize;
    int center = size/2;

    if(shape != mElementShape || size != mElementSize || mElement.empty())
    {
        mElement = cv::getStructuringElement(shape, cv::Size(size, size));
        mElementShape = shape;
        mElementSize = size;
    }

    cv::Mat element;
    if(mUserElement.hasValue()) {
        Image userElement = mUserElement;
        userElement.convertToGray();
        element = userElement;
    } else {
        element = mElement;
    }

    const cv::Mat src = mIn;
    cv::Mat dest;
    cv::morphologyEx(src, dest, mOperation, element, cv::Point(center, center), (int)mIterations);
    mOut.send(dest);
}
