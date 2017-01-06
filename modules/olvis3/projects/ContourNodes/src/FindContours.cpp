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

#include "FindContours.h"
#include <opencv2/imgproc.hpp>
REGISTER_FILTER(FindContours);

FindContours::FindContours()
{
    setName("FindContours");
    setDesc(QObject::tr("Extracts contours from an image"));
    setGroup("image/contour");

    mIn.setName("imageIn");
    mIn.setDesc(QObject::tr("Image input"));
    addInputPort(mIn);

    mOffsetIn.setName("offset");
    mOffsetIn.setDesc(QObject::tr("Offset that will be added to all calculated positions"));
    mOffsetIn.setDefault(QPointF(0.0, 0.0));
    addInputPort(mOffsetIn);

    mContourOut.setName("contour");
    addOutputListPort(mContourOut);
}

void FindContours::execute()
{
    Image input = mIn;
    input.convertToGray(CV_8U);

    cv::Mat image = input.clone();
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(image, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    for(uint i = 0; i< contours.size(); i++ )
    {
        if(hierarchy[i][3] >= 0) // this is an inner contour
            continue;
        std::vector<cv::Point> &contour = contours[i];
        QPointF offset = mOffsetIn;
        QPolygonF poly;
        for(uint j=0;j<contour.size(); j++)
            poly << (port::Point::qpoint(contour[j]) + offset);
        mContourOut.send(poly);
    }
}
