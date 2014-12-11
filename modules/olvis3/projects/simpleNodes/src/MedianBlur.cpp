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

#include "MedianBlur.h"
#include <opencv2/imgproc/imgproc.hpp>
REGISTER_FILTER(MedianBlur);

MedianBlur::MedianBlur()
{
    setName("Median");
    setDesc("Smoothes an image using median filter.");
    setGroup("image/noise");
}

void MedianBlur::execute()
{
    int kSize = mKSize;
    const cv::Mat src = mIn;
    cv::Mat dest;
    cv::medianBlur(src, dest, kSize);
    mOut.send(dest);
}
