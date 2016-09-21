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

#ifndef HILEC_POSE2D_H
#define HILEC_POSE2D_H

class Pose2d
{
public:
    Pose2d() : x(0), y(0), phi(0){}
    Pose2d(double x_, double y_, double phi_): x(x_), y(y_), phi(phi_){}
    Pose2d(const Pose2d &other) : x(other.x), y(other.y), phi(other.phi){}
    Pose2d& operator=(const Pose2d& other) { x = other.x; y = other.y; phi = other.phi; return*this; }

    double x, y, phi;
};

inline const Pose2d operator+(const Pose2d& p1, const Pose2d& p2)
{
    return Pose2d(p1.x + p2.x, p1.y + p2.y, p1.phi + p2.phi);
}

inline const Pose2d operator-(const Pose2d& p1, const Pose2d& p2)
{
    return Pose2d(p1.x - p2.x, p1.y - p2.y, p1.phi - p2.phi);
}

#endif // HILEC_POSE2D_H
