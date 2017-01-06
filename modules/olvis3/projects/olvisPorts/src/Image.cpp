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

#include <ports/Image.h>
#include <opencv2/imgproc.hpp>

using namespace cv;

Image::Image() : Mat()
{
}

Image::Image(const Image &m) : Mat(m)
{

}

Image::Image(const Mat &m) : Mat(m)
{

}

Image &Image::operator =(const Image &m)
{
    Mat::operator =(m);
    return *this;
}

Image &Image::operator =(const MatExpr &expr)
{
    Mat::operator =(expr);
    return *this;
}

void Image::convertToBit(int type)
{
    cv::Mat image_ = *this;
    cv::Mat image = image_;
    bool modified = false;

    if(type >= 0)
    {
        if(image.type() != type)
        {
            modified = true;
            image.convertTo(image, type, image.depth() == CV_16U ? 1/255.0 : 255.0);
        }
    }
    if(modified)
        Mat::operator = (image);

}

void Image::convertToGray(int depth)
{
    cv::Mat image_ = *this;
    cv::Mat image = image_;
    bool modified = false;

    if(depth >= 0)
    {
        if(image.depth() != depth)
        {
            modified = true;
            cv::Mat dest;
            image.convertTo(dest, depth, image.depth() == CV_16U ? 1/255.0 : 255.0);
            image = dest;
        }
    }

    if(image.channels() != 1)
    {
       modified = true;
       cv::Mat dest;
       if(image.channels() == 4)
           cv::cvtColor(image, dest, CV_RGBA2GRAY);
       else
           cv::cvtColor(image, dest, CV_RGB2GRAY);
       image = dest;
    }
    if(modified)
        Mat::operator =(image);
}

void Image::convertToRGB(int depth)
{
    cv::Mat image_ = *this;
    cv::Mat image = image_;
    bool modified = false;

    if(depth >= 0)
    {
        if(image.depth() != depth)
        {
            modified = true;
            cv::Mat dest;
            image.convertTo(dest, depth, image.depth() == CV_16U ? 1/255.0 : 255.0);
            image = dest;
        }
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
