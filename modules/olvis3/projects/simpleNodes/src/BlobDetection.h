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

#ifndef USER_BLOBDETECTION_H
#define USER_BLOBDETECTION_H

#include <filter/PluginInterface.h>
#include <ports/ImagePort.h>
#include <ports/RectPort.h>
#include <ports/PolygonPort.h>
#include <ports/PointPort.h>
#include <ports/PosePort.h>
#include <ports/IntegerPort.h>
#include <ports/BooleanPort.h>
#include <ports/RealPort.h>
#include <ports/LinePort.h>
#include <QSet>

class BlobDetection : public UserFilter
{
public:
    BlobDetection();
    virtual void execute();
protected:

    in::GrayImage mIn;
    in::Integer mMinPixels, mMaxPixels;
    in::Boolean mUseAxis;
    out::Integer mNumPixels;
    out::Real mPCe;
    in::Point mOffsetIn;
    out::Rect mRects;
    out::Polygon mPolygons;
    out::Pose2d mPoints;
    out::Line mLines;
};

#endif // USER_BLOBDETECTION_H
