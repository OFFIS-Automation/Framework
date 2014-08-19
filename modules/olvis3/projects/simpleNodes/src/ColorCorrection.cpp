// OFFIS Automation Framework
// Copyright (C) 2013 OFFIS e.V.
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

#include "ColorCorrection.h"

REGISTER_FILTER(ColorCorrection);

ColorCorrection::ColorCorrection()
{
    setName("ColorCorrection");
    setDesc("Multiplies each color with a factor.");
    setGroup("image/color");
    mIn.setName("input");
    mOut.setName("output");
    addInputPort(mIn);
    addOutputPort(mOut);
    mRFactor.setName("red gain");
    mRFactor.setDefault(1.0);
    mGFactor.setName("green gain");
    mGFactor.setDefault(1.0);
    mBFactor.setName("blue gain");
    mBFactor.setDefault(1.0);
    addInputPort(mRFactor);
    addInputPort(mGFactor);
    addInputPort(mBFactor);

}

void ColorCorrection::execute()
{
    double bGain = mBFactor.getValue();
    double gGain = mGFactor.getValue();
    double rGain = mRFactor.getValue();
    const cv::Mat source = mIn;
    std::vector<cv::Mat> channels;
    cv::split(source, channels);
    channels[0] *= bGain;
    channels[1] *= gGain;
    channels[2] *= rGain;
    cv::Mat dest;
    cv::merge(channels, dest);
    mOut.send(dest);
}
