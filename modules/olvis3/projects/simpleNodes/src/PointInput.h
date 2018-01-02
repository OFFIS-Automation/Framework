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

#ifndef POINTINPUT_H
#define POINTINPUT_H

#include <filter/PluginInterface.h>
#include <ports/PointPort.h>
#include <ports/BooleanPort.h>
#include <ports/PolygonPort.h>

class PointInput : public UserFilter
{
public:
    PointInput(bool list = false);
    virtual void execute();
	
protected:
    in::Point mIn;
    out::Point mOut;
    out::Point mSingleOut;
    out::Polygon mPoly;
    out::Boolean mUpdated;
    bool mAsList;
    QVector<QPointF> mLast;
};

class PointListInput  : public PointInput
{
public:
    PointListInput() : PointInput(true){}
};

#endif // POINTINPUT_H
