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

#include "MomentFilter.h"

#include <opencv2/imgproc/imgproc.hpp>

REGISTER_FILTER(MomentFilter);

MomentFilter::MomentFilter()
{
    setName("Moments");
    setDesc("Calculates the moments of a contour.");
    setGroup("image/contour");
    mIn.setName("input");
    mNumPixelsOut.setName("numPixels");
    mNumPixelsOut.setDesc("The number of pixels inside a contour(m00)");
    mCogOut.setName("cog");
    mCogOut.setDesc("The center of gravity (based on m01, m10 and m00");
    addInputListPort(mIn);
    addOutputListPort(mNumPixelsOut);
    addOutputListPort(mCogOut);
}

void MomentFilter::execute()
{
    while(mIn.hasValue())
    {
        std::vector<cv::Point> contour = mIn;
        cv::Moments mom = cv::moments(contour, true);
        if(mom.m00 > 0)
        {
            mNumPixelsOut.send(mom.m00);
            mCogOut.send(QPointF(mom.m10/mom.m00, mom.m01 / mom.m00));
        }
    }
}
