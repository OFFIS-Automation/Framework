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

#include <ports/DepthMap.h>
#include <opencv2/imgproc.hpp>

using namespace cv;
DepthMap::DepthMap() : Mat(),
    muMPerPixel(-1)
{
}

DepthMap::DepthMap(int rows, int cols) : Mat(rows, cols, CV_64FC1),
    muMPerPixel(-1)
{
}

DepthMap::DepthMap(const DepthMap &m) : Mat(m),
    muMPerPixel(m.umPerPixel())
{
}

DepthMap::DepthMap(const DepthMap &m, const cv::Range &rowRange, const cv::Range &colRange)
    : Mat(m, rowRange, colRange),
      muMPerPixel(m.umPerPixel())
{
}

DepthMap::DepthMap(const DepthMap &m, const cv::Rect &roi) : Mat(m, roi),
    muMPerPixel(m.umPerPixel())
{
}

DepthMap::DepthMap(const cv::Mat &m) : Mat(m),
    muMPerPixel(-1)
{
    makeDepthMap();
}

DepthMap::DepthMap(const IplImage *img) : Mat(cv::cvarrToMat(img)),
    muMPerPixel(-1)
{
    makeDepthMap();
}


DepthMap::DepthMap(cv::Size size) : Mat(size, CV_64FC1),
    muMPerPixel(-1)
{
}

DepthMap::DepthMap(cv::Size size, unsigned char *data, size_t step) : Mat(size, CV_64FC1, data, step),
    muMPerPixel(-1)
{
}

DepthMap::DepthMap(int rows, int cols, unsigned char *data, size_t step) : Mat(rows, cols, CV_64FC1, data, step),
    muMPerPixel(-1)
{
}

DepthMap::DepthMap(const MatExpr &expr) : Mat(expr),
    muMPerPixel(-1)
{
    makeDepthMap();
}

void DepthMap::makeDepthMap()
{
    cv::Mat image_ = *this;
    cv::Mat image = image_;
    bool modified = false;
    if(image.depth() != CV_64F)
    {
        modified = true;
        image_.convertTo(image, CV_64FC1);
    }
    if(image.channels() != 1)
    {
        std::vector<cv::Mat> vec(image.channels());
        cv::split(image_, vec);
        image = vec[0];
        modified = true;

    }
    if(modified)
        Mat::operator =(image);
}


DepthMap& DepthMap::operator=(const DepthMap& m)
{
    Mat::operator =(m);
    setUMPerPixel(m.umPerPixel());
    return *this;
}

DepthMap& DepthMap::operator=(const MatExpr& exp)
{
    Mat::operator =(exp);
    setUMPerPixel(-1);
    makeDepthMap();
    return *this;
}

Image DepthMap::toImage()
{
    Image img;
    double minVal, maxVal;
    cv::minMaxLoc(*this, &minVal, &maxVal);
    double factor = 255.0/(maxVal- minVal);
    convertTo(img, CV_8U, factor, -minVal*factor);
    return img;
}

