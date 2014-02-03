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

#ifndef RESIZEIMAGE_H
#define RESIZEIMAGE_H

#include <filter/PluginInterface.h>
#include <ports/ImagePort.h>
#include <ports/BooleanPort.h>
#include <ports/IntegerPort.h>

class ResizeImage : public UserFilter
{
public:
    ResizeImage();
    void execute();
protected:
    in::Image mIn;
    in::Boolean mConstrainToWidth;
    in::Boolean mConstrainToHeight;
    in::Integer mWidth, mHeight;

    out::Image mOut;
};

#endif // RESIZEIMAGE_H
