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

#ifndef RgbImage_H
#define RgbImage_H

#include "DefaultPortsGlobal.h"
#include <opencv2/core/core.hpp>

class DEFAULTPORTSSHARED_EXPORT RgbImage : public cv::Mat
{
public:
    RgbImage();
    RgbImage(int rows, int cols);
    RgbImage(cv::Size size);
    // copy constructor
    RgbImage(const RgbImage& m);
    RgbImage(const cv::Mat& m);
    // constructor for matrix headers pointing to user-allocated data
    RgbImage(int rows, int cols, unsigned char* data, size_t step=AUTO_STEP);
    RgbImage(cv::Size size, unsigned char* data, size_t step=AUTO_STEP);
    // creates a matrix header for a part of the bigger matrix
    RgbImage(const RgbImage& m, const cv::Range& rowRange, const cv::Range& colRange);
    RgbImage(const RgbImage& m, const cv::Rect& roi);
    // converts old-style CvMat to the new matrix; the data is not copied by default
    RgbImage(const CvMat* m, bool copyData=false);
    // converts old-style IplImage to the new matrix; the data is not copied by default
    RgbImage(const IplImage* img, bool copyData=false);
    // helper constructor to compile matrix expressions
    RgbImage(const cv::MatExpr& expr);
    // assignment operators
    RgbImage& operator = (const RgbImage& m);
    RgbImage& operator = (const cv::MatExpr& expr);
protected:
    void makeRgb();
};

#endif // RgbImage_H
