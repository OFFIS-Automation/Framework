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

#include <ports/RgbaImage.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
RgbaImage::RgbaImage() : Mat()
{
}

RgbaImage::RgbaImage(int rows, int cols) : Mat(rows, cols, CV_8UC4)
{
}

RgbaImage::RgbaImage(const CvMat *m, bool copyData) : Mat(m ,copyData)
{
    makeRgba();
}

RgbaImage::RgbaImage(const RgbaImage &m) : Mat(m)
{
}

RgbaImage::RgbaImage(const RgbaImage &m, const cv::Range &rowRange, const cv::Range &colRange)
    : Mat(m, rowRange, colRange)
{
}

RgbaImage::RgbaImage(const RgbaImage &m, const cv::Rect &roi) : Mat(m, roi)
{
}

RgbaImage::RgbaImage(const cv::Mat &m) : Mat(m)
{
    makeRgba();
}

RgbaImage::RgbaImage(const IplImage *img, bool copyData) : Mat(img, copyData)
{
    makeRgba();
}


RgbaImage::RgbaImage(cv::Size size) : Mat(size, CV_8UC4)
{
}

RgbaImage::RgbaImage(cv::Size size, unsigned char *data, size_t step) : Mat(size, CV_8UC4, data, step)
{
}

RgbaImage::RgbaImage(int rows, int cols, unsigned char *data, size_t step) : Mat(rows, cols, CV_8UC4, data, step)
{
}

RgbaImage::RgbaImage(const MatExpr &expr) : Mat(expr)
{
    makeRgba();
}
void RgbaImage::makeRgba()
{
    cv::Mat image_ = *this;
    cv::Mat image = image_;
    bool modified = false;
    if(image.depth() != CV_8U)
    {
        modified = true;
        image_.convertTo(image, CV_8U);
    }
    if(image.channels() != 4)
    {
        modified = true;
        cv::Mat dest;
        if(image.channels() == 3)
            cv::cvtColor(image, dest, CV_RGB2RGBA);
        else
            cv::cvtColor(image, dest, CV_GRAY2RGBA);
        image = dest;
    }
    if(modified)
        Mat::operator =(image);
}

RgbaImage& RgbaImage::operator=(const RgbaImage& m)
{
    Mat::operator =(m);
    makeRgba();
    return *this;
}


RgbaImage& RgbaImage::operator=(const MatExpr& exp)
{
    Mat::operator =(exp);
    makeRgba();
    return *this;
}

