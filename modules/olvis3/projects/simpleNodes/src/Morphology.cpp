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

#include "Morphology.h"
#include <QDebug>
#include <opencv2/imgproc/imgproc.hpp>

REGISTER_FILTER(Morphology);

Morphology::Morphology()
{
    setName("Morphology");
    setGroup("image/convert");

    mOperation.setName("operation");
    mOperation.setDesc("Perform the selected morphology operation.");
    mOperation.addChoice(cv::MORPH_ERODE, "erode");
    mOperation.addChoice(cv::MORPH_DILATE, "dilate");
    mOperation.addChoice(cv::MORPH_OPEN, "open");
    mOperation.addChoice(cv::MORPH_CLOSE, "close");
    mOperation.addChoice(cv::MORPH_GRADIENT, "gradient");
    mOperation.addChoice(cv::MORPH_TOPHAT, "top hat");
    mOperation.addChoice(cv::MORPH_BLACKHAT, "black hat");
    addInputPort(mOperation);

    mIterations.setName("iterations");
    mIterations.setDesc("Number of iterations to perform.");
    mIterations.setDefault(1);
    mIterations.setRange(1, 255);
    addInputPort(mIterations);

    mElementType.setName("element");
    mElementType.setDesc("Element type");
    mElementType.setDefault(cv::MORPH_RECT);
    mElementType.addChoice(cv::MORPH_RECT, "Rectangle");
    mElementType.addChoice(cv::MORPH_ELLIPSE, "Ellipse");
    mElementType.addChoice(cv::MORPH_CROSS, "Cross");
    addInputPort(mElementType);

    mUserElement.setName("specialElement");
    mUserElement.setDesc("Special structuring element, overrides element and size if connected.");
    mUserElement.setMode(OptionalPortMode);
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
    if(mUserElement.hasValue())
        element = mUserElement;
    else
        element = mElement;


    const cv::Mat src = mIn;
    cv::Mat dest;
    cv::morphologyEx(src, dest, mOperation, element, cv::Point(center, center), (int)mIterations);
    mOut.send(dest);
}
