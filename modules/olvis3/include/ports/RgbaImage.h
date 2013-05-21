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

#ifndef RGBAIMAGE_H
#define RGBAIMAGE_H
#include "DefaultPortsGlobal.h"
#include <opencv2/core/core.hpp>

class DEFAULTPORTSSHARED_EXPORT RgbaImage : public cv::Mat
{
public:
    RgbaImage();
    RgbaImage(int rows, int cols);
    RgbaImage(cv::Size size);
    // copy constructor
    RgbaImage(const RgbaImage& m);
    RgbaImage(const cv::Mat& m);
    // constructor for matrix headers pointing to user-allocated data
    RgbaImage(int rows, int cols, unsigned char* data, size_t step=AUTO_STEP);
    RgbaImage(cv::Size size, unsigned char* data, size_t step=AUTO_STEP);
    // creates a matrix header for a part of the bigger matrix
    RgbaImage(const RgbaImage& m, const cv::Range& rowRange, const cv::Range& colRange);
    RgbaImage(const RgbaImage& m, const cv::Rect& roi);
    // converts old-style CvMat to the new matrix; the data is not copied by default
    RgbaImage(const CvMat* m, bool copyData=false);
    // converts old-style IplImage to the new matrix; the data is not copied by default
    RgbaImage(const IplImage* img, bool copyData=false);
    // helper constructor to compile matrix expressions
    RgbaImage(const cv::MatExpr& expr);
    // assignment operators
    RgbaImage& operator = (const RgbaImage& m);
    RgbaImage& operator = (const cv::MatExpr& expr);
protected:
    void makeRgba();
};

#endif // RGBAIMAGE_H
