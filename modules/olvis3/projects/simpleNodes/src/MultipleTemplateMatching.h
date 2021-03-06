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

#ifndef MULTIPLETEMPLATEMATCHING_H
#define MULTIPLETEMPLATEMATCHING_H

#include <filter/PluginInterface.h>
#include <ports/ImagePort.h>
#include <ports/IntegerPort.h>
#include <ports/PointPort.h>
#include <ports/RealPort.h>
#include <ports/BooleanPort.h>
#include <ports/RectPort.h>

class MultipleTemplateMatching : public UserFilter
{
public:
    MultipleTemplateMatching();
    void execute();

protected:
    in::Image mIn, mTemplateIn;
    in::Integer mMethodIn;
    in::Real mMinScoreIn;
    in::Boolean mSendResultImage, mNeedNewImage;
    in::Point mOffsetIn;

    out::Point mPositionOuts;
    out::Real mScoreOuts;
    out::Image mOut;
};

#endif // MULTIPLETEMPLATEMATCHING_H
