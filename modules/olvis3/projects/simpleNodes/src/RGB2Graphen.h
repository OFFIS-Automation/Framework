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

#ifndef RGB2GRAPHEN_H
#define RGB2GRAPHEN_H
#include <filter/PluginInterface.h>
#include <ports/ImagePort.h>
#include <ports/IntegerPort.h>
#include <ports/RealPort.h>
#include <ports/BooleanPort.h>
#include <ports/StringPort.h>

#include <string>

class Rgb2Graphen : public UserFilter
{
public:
    Rgb2Graphen();
    void execute();
    std::vector<double> getDoubleVariance() const;
    void setDoubleVariance(const std::vector<double> &value);

protected:
    // defining enum of modes of this filter
    enum Mode
    {
        BlueSubstractDefault = 0,
        BlueSubstractTweeked,
        WeightedSummPaper,
        GaussSingle,
        GaussCombined
    };

    // defining enum of target options
    enum Target
    {
        SingleLayerGraphen = 0,
        DoubleLayerGraphen
    };

    // image input
    in::RgbImage mIn;
    // mode option
    in::Integer mMode;
    in::Integer mTarget;

    // graphene RGB values
    in::String mSingleRGBString;
    in::String mSingleVarianceString;
    in::String mDoubleRGBString;
    in::String mDoubleVarianceString;

    // debug option
    in::Real mVariance;
    in::Integer mChannel;

    // output image
    out::GrayImage mOut;

private:
    // variables to hold the configured RGB values and
    // variances of the single/double layer graphene
    std::vector<double> mSingleRGBVector;
    std::vector<double> mSingleVarianceVector;
    std::vector<double> mDoubleRGBVector;
    std::vector<double> mDoubleVarianceVector;

    // internal implementation of a gauss function
    cv::Mat gauss(cv::Mat matrix, double s, double m);
    // spliting comma seperated values of a string and
    // converts them into double
    std::vector<double> splitString(std::string str);
    // loads the new set configuration of graphene values
    // to reduce string coversions to a minimum
    void loadRGBConfig();
};


#endif // RGB2GRAPHEN_H
