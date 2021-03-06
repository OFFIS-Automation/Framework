// OFFIS Automation Framework
// Copyright (C) 2013-2018 OFFIS e.V.
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

#include "GaussianBlur.h"
#include <opencv2/imgproc.hpp>

REGISTER_FILTER(GaussianBlur);
GaussianBlur::GaussianBlur()
{
    setName("Gaussian");
    setDesc(QObject::tr("Smoothes an image using a gaussian filter"));
    setGroup("image/smoothing");
}

void GaussianBlur::execute()
{
    int kSize = mKSize;
    const cv::Mat src = mIn;
    cv::Mat dest;
    cv::GaussianBlur(src, dest, cv::Size(kSize, kSize), 0, 0, 0);
    mOut.send(dest);
}
