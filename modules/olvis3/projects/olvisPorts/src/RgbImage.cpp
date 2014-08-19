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

#include <ports/RgbImage.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
RgbImage::RgbImage() : Mat()
{
}

RgbImage::RgbImage(int rows, int cols) : Mat(rows, cols, CV_8UC3)
{
}

RgbImage::RgbImage(const CvMat *m, bool copyData) : Mat(m ,copyData)
{
    makeRgb();
}

RgbImage::RgbImage(const RgbImage &m) : Mat(m)
{
}

RgbImage::RgbImage(const RgbImage &m, const cv::Range &rowRange, const cv::Range &colRange)
    : Mat(m, rowRange, colRange)
{
}

RgbImage::RgbImage(const RgbImage &m, const cv::Rect &roi) : Mat(m, roi)
{
}

RgbImage::RgbImage(const cv::Mat &m) : Mat(m)
{
    makeRgb();
}

RgbImage::RgbImage(const IplImage *img, bool copyData) : Mat(img, copyData)
{
    makeRgb();
}


RgbImage::RgbImage(cv::Size size) : Mat(size, CV_8UC3)
{
}

RgbImage::RgbImage(cv::Size size, unsigned char *data, size_t step) : Mat(size, CV_8UC3, data, step)
{
}

RgbImage::RgbImage(int rows, int cols, unsigned char *data, size_t step) : Mat(rows, cols, CV_8UC3, data, step)
{
}

RgbImage::RgbImage(const MatExpr &expr) : Mat(expr)
{
    makeRgb();
}
void RgbImage::makeRgb()
{
    cv::Mat image_ = *this;
    cv::Mat image = image_;
    bool modified = false;
    if(image.depth() != CV_8U)
    {
        modified = true;
        image_.convertTo(image, CV_8U);
    }
    if(image.channels() != 3)
    {
        modified = true;
        cv::Mat dest;
        if(image.channels() == 4)
            cv::cvtColor(image, dest, CV_RGBA2RGB);
        else
            cv::cvtColor(image, dest, CV_GRAY2RGB);
        image = dest;
    }
    if(modified)
        Mat::operator =(image);
}

RgbImage& RgbImage::operator=(const RgbImage& m)
{
    Mat::operator =(m);
    makeRgb();
    return *this;
}


RgbImage& RgbImage::operator=(const MatExpr& exp)
{
    Mat::operator =(exp);
    makeRgb();
    return *this;
}
