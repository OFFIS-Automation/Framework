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

#ifndef DEPTHMAP_H
#define DEPTHMAP_H

#include "GrayImage.h"

class DEFAULTPORTSSHARED_EXPORT DepthMap : public cv::Mat
{
public:
    DepthMap();
    DepthMap(int rows, int cols);
    DepthMap(cv::Size size);
    // copy constructor
    DepthMap(const DepthMap& m);
    DepthMap(const cv::Mat& m);
    // constructor for matrix headers pointing to user-allocated data
    DepthMap(int rows, int cols, unsigned char* data, size_t step=AUTO_STEP);
    DepthMap(cv::Size size, unsigned char* data, size_t step=AUTO_STEP);
    // creates a matrix header for a part of the bigger matrix
    DepthMap(const DepthMap& m, const cv::Range& rowRange, const cv::Range& colRange);
    DepthMap(const DepthMap& m, const cv::Rect& roi);
    // converts old-style CvMat to the new matrix; the data is not copied by default
    DepthMap(const CvMat* m, bool copyData=false);
    // converts old-style IplImage to the new matrix; the data is not copied by default
    DepthMap(const IplImage* img, bool copyData=false);

    // helper constructor to compile matrix expressions
    DepthMap(const cv::MatExpr& expr);
    // assignment operators
    DepthMap& operator = (const DepthMap& m);
    DepthMap& operator = (const cv::MatExpr& expr);
    void setUMPerPixel(double umPerPixel) { muMPerPixel = umPerPixel; }
    double umPerPixel() const { return muMPerPixel; }
    GrayImage toImage();
    operator GrayImage() { return toImage(); }

    double* ptr(int i = 0)  { return cv::Mat::ptr<double>(i); }
    const double* ptr(int i = 0)  const { return cv::Mat::ptr<double>(i); }

protected:
    void makeDepthMap();
    double muMPerPixel;
};

#endif // DEPTHMAP_H
