// OFFIS Automation Framework
// Copyright (C) 2013-2018 OFFIS e.V.
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

#ifndef IMAGE_H
#define IMAGE_H

#include "DefaultPortsGlobal.h"
#include <opencv2/core.hpp>


class DEFAULTPORTSSHARED_EXPORT Image : public cv::Mat
{
public:
    Image();

    // copy constructor
    Image(const Image& m);
    Image(const cv::Mat& m);

    // assignment operators
    Image& operator = (const Image& m);
    Image& operator = (const cv::MatExpr& expr);

    // Helper, -1 will keep type
    void convertToDepth(int depth = -1);
    void convertToGray(int depth = -1);
    void convertToRGB(int depth = -1);
};

#endif // GRAYIMAGE_H
