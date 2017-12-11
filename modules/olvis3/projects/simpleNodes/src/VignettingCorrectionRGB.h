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

#ifndef USER_VIGNETTINGCORRECTIONRGB_H
#define USER_VIGNETTINGCORRECTIONRGB_H

#include <filter/PluginInterface.h>
#include <ports/ImagePort.h>

#include <ports/IntegerPort.h>
#include <ports/StringPort.h>

class VignettingCorrectionRGB : public UserFilter
{
public:
    VignettingCorrectionRGB();
    virtual void execute();

protected:
    enum Mode {
        Difference = 0,
        Ratio
    };

    in::Image mIn;
    in::Image mVignetteIn;

    in::Integer mMode;
    in::String mStringOffset;

    out::Image mOut;
private:
    std::vector<double> mOffsetVector;
    std::vector<double> splitString(std::string str);

};

#endif // USER_VIGNETTINGCORRECTIONRGB_H
