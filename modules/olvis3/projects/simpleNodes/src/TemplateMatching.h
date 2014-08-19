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

#ifndef TEMPLATEMATCHING_H
#define TEMPLATEMATCHING_H

#include <filter/PluginInterface.h>
#include <ports/ImagePort.h>
#include <ports/IntegerPort.h>
#include <ports/PointPort.h>
#include <ports/RealPort.h>
#include <ports/BooleanPort.h>
#include <ports/RectPort.h>

class TemplateMatching : public UserFilter
{
public:
    TemplateMatching();
    void start();
    void execute();

protected:
    in::Image mImageIn, mTemplateIn;
    in::Integer mMethodIn;
    in::Real mMinScoreIn;
    in::Boolean mSendInvalidPos;
    in::Boolean mSendResultImage, mNeedNewImage;
    in::Integer mFastSearchRange;

    out::Rect mFastSearchOut;
    out::Point mPositionOut;
    out::Real mScoreOut;
    out::GrayImage mResultOut;
    out::Point mResultOffset;

    cv::Point mLastPos;
};

#endif // TEMPLATEMATCHING_H
