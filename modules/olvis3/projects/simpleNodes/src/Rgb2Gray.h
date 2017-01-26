// OFFIS Automation Framework
// Copyright (C) 2013-2017 OFFIS e.V.
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

#ifndef RGB2GRAY_H
#define RGB2GRAY_H

#include <filter/PluginInterface.h>
#include <ports/ImagePort.h>
#include <ports/IntegerPort.h>
#include <ports/BooleanPort.h>

class Rgb2Gray : public UserFilter
{
public:
    Rgb2Gray();
    void execute();
	
protected:
    enum Mode
    {
        BlueMode = 0,
        GreenMode,
        RedMode,
        BlueSubstractMode,
        GreenSubstractMode,
        RedSubstractMode,
        GrayscaleMode,
        MinMode,
        MaxMode
    };

    in::Image mIn;
    in::Integer mMode;
    in::Boolean mSubstract;
    out::Image mOut;
};

#endif // RGB2GRAY_H
