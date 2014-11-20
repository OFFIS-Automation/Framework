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

#include <ports/GrayImage.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
GrayImage::GrayImage() : Mat()
{
}

GrayImage::GrayImage(int rows, int cols) : Mat(rows, cols, CV_8UC1)
{
}

GrayImage::GrayImage(const CvMat *m, bool copyData) : Mat(m ,copyData)
{
    makeGrayscale();
}

GrayImage::GrayImage(const GrayImage &m) : Mat(m)
{
}

GrayImage::GrayImage(const GrayImage &m, const cv::Range &rowRange, const cv::Range &colRange)
    : Mat(m, rowRange, colRange)
{
}

GrayImage::GrayImage(const GrayImage &m, const cv::Rect &roi) : Mat(m, roi)
{
}

GrayImage::GrayImage(const cv::Mat &m) : Mat(m)
{
    makeGrayscale();
}

GrayImage::GrayImage(const IplImage *img, bool copyData) : Mat(img, copyData)
{
    makeGrayscale();
}


GrayImage::GrayImage(cv::Size size) : Mat(size, CV_8UC1)
{
}

GrayImage::GrayImage(cv::Size size, unsigned char *data, size_t step) : Mat(size, CV_8UC1, data, step)
{
}

GrayImage::GrayImage(int rows, int cols, unsigned char *data, size_t step) : Mat(rows, cols, CV_8UC1, data, step)
{
}

GrayImage::GrayImage(const MatExpr &expr) : Mat(expr)
{
    makeGrayscale();
}

void GrayImage::makeGrayscale()
{
    cv::Mat image_ = *this;
    cv::Mat image = image_;
    bool modified = false;
    if(image.depth() != CV_8U)
    {
        modified = true;
        image_.convertTo(image, CV_8U);
    }
    if(image.channels() != 1)
    {
        modified = true;
        cv::Mat dest;
        cv::cvtColor(image, dest, CV_RGB2GRAY);
        image = dest;
    }
    if(modified)
        Mat::operator =(image);
}


GrayImage& GrayImage::operator=(const GrayImage& m)
{
    Mat::operator =(m);
    makeGrayscale();
    return *this;
}

GrayImage& GrayImage::operator=(const MatExpr& exp)
{
    Mat::operator =(exp);
    makeGrayscale();
    return *this;
}
