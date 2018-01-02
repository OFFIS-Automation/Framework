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

#ifndef INPAINTING_H
#define INPAINTING_H

#include <filter/PluginInterface.h>
#include <ports/ImagePort.h>
#include <ports/IntegerPort.h>

class Inpainting : public UserFilter
{
public:
    Inpainting();
    void execute();

protected:

    enum Mode
    {
        NS = 0,
        Telea
    };

    in::Integer mMode;
    in::Integer mRadius;
    in::Image mIn;
    in::Image mInMask;
    out::Image mOut;
};

#endif // INPAINTING_H

