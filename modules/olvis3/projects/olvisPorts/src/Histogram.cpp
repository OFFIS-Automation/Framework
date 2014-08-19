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

#include <ports/Histogram.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
Histogram::Histogram() : Mat()
{
}

Histogram::Histogram(int rows) : Mat(rows, 1, CV_8UC1)
{
}

Histogram::Histogram(const CvMat *m, bool copyData) : Mat(m ,copyData)
{
    makeHistogram();
}

Histogram::Histogram(const Histogram &m) : Mat(m)
{
}

Histogram::Histogram(const cv::Mat &m) : Mat(m)
{
    makeHistogram();
}

Histogram::Histogram(const IplImage *img, bool copyData) : Mat(img, copyData)
{
    makeHistogram();
}


Histogram::Histogram(int rows, unsigned char *data, size_t step) : Mat(rows, 1, CV_8UC1, data, step)
{
}

Histogram::Histogram(const MatExpr &expr) : Mat(expr)
{
    makeHistogram();
}

void Histogram::makeHistogram()
{
    cv::Mat image_ = *this;
    cv::Mat image = image_;
    bool modified = false;
    if(image.depth() != CV_32F)
    {
        modified = true;
        image_.convertTo(image, CV_32F);
    }
    if(image.cols > 1)
    {
        image = image_(cv::Rect(0,0,1, image.rows));
        modified = true;
    }
    if(modified)
        Mat::operator =(image);
}


Histogram& Histogram::operator=(const Histogram& m)
{
    Mat::operator =(m);
    makeHistogram();
    return *this;
}

Histogram& Histogram::operator=(const MatExpr& exp)
{
    Mat::operator =(exp);
    makeHistogram();
    return *this;
}

