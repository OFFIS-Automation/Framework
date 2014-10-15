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

#include "DepthMap2Image.h"

REGISTER_FILTER(DepthMap2Image);

DepthMap2Image::DepthMap2Image()
{
    setName("DepthMap2Image");
    setDesc("Creates a grayscale image from the DepthMap");
    setGroup("depth map");
    mIn.setName("input");
    mOut.setName("output");
    addInputPort(mIn);
    addOutputPort(mOut);
}

void DepthMap2Image::execute()
{
    DepthMap input = mIn;
    mOut.send(input.toImage());
}
