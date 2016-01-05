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

#ifndef POSE2D_H
#define POSE2D_H

#include "DefaultPortsGlobal.h"

class DEFAULTPORTSSHARED_EXPORT Pose2d
{
public:
    Pose2d();
    Pose2d(double x, double y ,double phi);
    Pose2d(const Pose2d& other);
    const double x, y, phi;
};

const Pose2d DEFAULTPORTSSHARED_EXPORT operator+(const Pose2d& p1, const Pose2d& p2);
const Pose2d DEFAULTPORTSSHARED_EXPORT operator-(const Pose2d& p1, const Pose2d& p2);

#endif // POSE2D_H
