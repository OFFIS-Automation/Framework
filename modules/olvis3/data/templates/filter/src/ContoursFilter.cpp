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

#include "ContoursFilter.h"
#include <opencv2/imgproc/imgproc.hpp>
REGISTER_FILTER(ContoursFilter);

ContoursFilter::ContoursFilter()
{
    setName("ContoursFilter");
    setDesc("Extracts contours from an image. The image should be thresholded");
    setGroup("user");
    
	mIn.setName("input");
    mIn.setDesc("Thresholded input image");
    addInputPort(mIn);
	
	mOffsetIn.setName("offset");
    mOffsetIn.setDesc("Offset that will be added to all calculated positions");
    mOffsetIn.setDefault(QPointF(0.0, 0.0));
	addInputPort(mOffsetIn);
	
    mContourOut.setName("contour");
    mContourOut.setDesc("A list of the calculated outer contours");
    addOutputListPort(mContourOut);
}

void ContoursFilter::execute()
{
    const cv::Mat input = mIn;
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
