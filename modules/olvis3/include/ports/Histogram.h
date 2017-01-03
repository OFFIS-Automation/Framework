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

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "GrayImage.h"

class DEFAULTPORTSSHARED_EXPORT Histogram : public cv::Mat
{
public:
    Histogram();
    Histogram(int rows);
    // copy constructor
    Histogram(const Histogram& m);
    Histogram(const cv::Mat& m);
    // constructor for matrix headers pointing to user-allocated data
    Histogram(int rows, unsigned char* data, size_t step=AUTO_STEP);
    // converts old-style IplImage to the new matrix; the data is not copied by default
    Histogram(const IplImage* img);

    // helper constructor to compile matrix expressions
    Histogram(const cv::MatExpr& expr);
    // assignment operators
    Histogram& operator = (const Histogram& m);
    Histogram& operator = (const cv::MatExpr& expr);

protected:
    void makeHistogram();
};

#endif // HISTOGRAM_H
