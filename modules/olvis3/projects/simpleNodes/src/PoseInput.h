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

#ifndef POSEINPUT_H
#define POSEINPUT_H

#include <filter/PluginInterface.h>
#include <ports/PosePort.h>
#include <ports/BooleanPort.h>
#include <ports/PolygonPort.h>

class PoseInput : public UserFilter
{
public:
    PoseInput();
    virtual void execute();
protected:
    in::Pose2d mIn;
    out::Pose2d mOut;
    out::Boolean mUpdated;
};

#endif // POSEINPUT_H
