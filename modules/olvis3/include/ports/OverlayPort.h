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

#ifndef OVERLAYPORT_H
#define OVERLAYPORT_H

#include "../filter/Port.h"

/**
 * a specialization of Port with advanced UI control
 **/
class OverlayPort : public Port
{
public:
    /**
     * forces the port to be invisible in the image view as input
     * this is usefull if a port does not directly correspond to
     * the image output of the filter, e.g. if a roi changes the image output,
     * it should not be directly drawn on the output and therefore not visible
    **/
    void setDisplayVisibility(bool visible)
    {
        setConstraint("displayVisibility", visible);
    }

protected:
    OverlayPort(int id, const QString &name, bool isMain = false)
        : Port(id, name, isMain)
    {
    }
};

#endif // OVERLAYPORT_H
