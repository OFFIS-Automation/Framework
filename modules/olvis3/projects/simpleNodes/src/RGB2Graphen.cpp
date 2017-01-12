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

#include "Rgb2Graphen.h"
#include <opencv2/imgproc.hpp>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

REGISTER_FILTER(Rgb2Graphen);


Rgb2Graphen::Rgb2Graphen()
{
    setName("RGB2Graphen");
    setDesc(QObject::tr("Extracts graphene layers from an RGB image"));
    setGroup("application/graphene");

    // define image input
    mIn.setName("imageIn");
    mIn.setDesc(QObject::tr("Image input"));
    addInputPort(mIn);

    // define mode selection menu
    mMode.setName("mode");
    mMode.setDesc(QObject::tr("Select the operation mode"));
    mMode.setIcon(QImage(":/SimpleNodes/colorMode.png"));
    // define choices
    mMode.addChoice(BlueSubstractDefault, "B - R/2 - G/2 - |R - G|/2");
    mMode.addChoice(WeightedSummPaper, "0.3*R + 0.59*G + 0.11*B");
    mMode.addChoice(BlueSubstractTweeked, "(B - R) + (B - G)");
    mMode.addChoice(GaussSingle, "Gauss weighted image for targets");
    mMode.addChoice(GaussCombined, "Gauss weighted image for all targets combined");
    addInputPort(mMode);

    // define target selection menu
    mTarget.setName("target");
    mTarget.setDesc(QObject::tr("Set the targeted graphene"));
    mTarget.setIcon(QImage(":/SimpleNodes/colorMode.png"));
    // define choices
    mTarget.addChoice(SingleLayerGraphen, tr("Single layer graphene"));
    mTarget.addChoice(DoubleLayerGraphen, tr("Double layer graphene"));
    addInputPort(mTarget);

    // multiplicator for the variance values
    mVariance.setName("variance modifier");
    mVariance.setDefault(1);
    addInputPort(mVariance);

    // define RGB inputs
    mSingleRGBString.setName("single layer RGB");
    mSingleRGBString.setDesc(QObject::tr("Defines the searched RGB value in R,G,B format"));
    mSingleRGBString.setDefault("66,67,72");
    addInputPort(mSingleRGBString);

    mDoubleRGBString.setName("double layer RGB");
    mDoubleRGBString.setDesc(QObject::tr("Defines the searched RGB value in R,G,B format"));
    mDoubleRGBString.setDefault("56,57,70");
    addInputPort(mDoubleRGBString);

    mSingleVarianceString.setName("single layer variance");
    mSingleVarianceString.setDesc(QObject::tr("Defines the variance of the searched RGB value in R,G,B format"));
    mSingleVarianceString.setDefault("2.23423,1.56613,1.55053");
    addInputPort(mSingleVarianceString);

    mDoubleVarianceString.setName("double layer variance");
    mDoubleVarianceString.setDesc(QObject::tr("Defines the variance of the searched RGB value in R,G,B format."));
    mDoubleVarianceString.setDefault("2.57551,2.23036,1.58594");
    addInputPort(mDoubleVarianceString);

    mChannel.setName("channel");
    mChannel.addChoice(0, "0");
    mChannel.addChoice(1, "1");
    mChannel.addChoice(2, "2");
    addInputPort(mChannel);

    // define image output
    mOut.setName("imageOut");
    mOut.setDesc(QObject::tr("Image output"));
    addOutputPort(mOut);
}

std::vector<double> Rgb2Graphen::splitString(std::string str)
{
    std::vector<double> values;
    size_t pos = 0;

    while ((pos = str.find(',', 1)) != std::string::npos)
    {
        std::string p = str.substr(0, pos);
        values.push_back(stod(p));
        str = str.substr(pos + 1);
    }

    if (!str.empty()) {
        values.push_back(stod(str));
    }
    return (values);
}

void Rgb2Graphen::loadRGBConfig()
{
    std::vector<double> singleRGBcache;
    std::vector<double> doubleRGBcache;
    std::vector<double> singleVariancecache;
    std::vector<double> doubleVariancecache;

    // split configuration and test if correct
    singleRGBcache = splitString(mSingleRGBString.getValue().toStdString());
    if (singleRGBcache.size() != 3)
        return;
    doubleRGBcache = splitString(mDoubleRGBString.getValue().toStdString());
    if (doubleRGBcache.size() != 3)
        return;
    singleVariancecache = splitString(mSingleVarianceString.getValue().toStdString());
    if (singleVariancecache.size() != 3)
        return;
    doubleVariancecache = splitString(mDoubleVarianceString.getValue().toStdString());
    if (doubleVariancecache.size() != 3)
        return;

    // confiuration has the right size. Use new config
    Rgb2Graphen::mSingleRGBVector = singleRGBcache;
    Rgb2Graphen::mDoubleRGBVector = doubleRGBcache;
    Rgb2Graphen::mSingleVarianceVector = singleVariancecache;
    Rgb2Graphen::mDoubleVarianceVector = doubleVariancecache;
}

cv::Mat Rgb2Graphen::gauss(cv::Mat matrix, double s, double m)
{
    cv::Mat dest;
    matrix.convertTo(dest, CV_64F);

    // x-u
    cv::subtract(dest, m, dest);
    // (x-u)^2
    cv::pow(dest, 2, dest);
    // -(x-u)^2
    dest *= -1;
    // -(x-u)^2/2s^2
    dest /= 2*pow(s,2);
    // e^(-(x-u)^2/2s^2)
    cv::exp(dest, dest);
    // 1/(2*PI*s^2) * e^(-(x-u)^2/2s^2)
    //dest *= 1/sqrt(2 * M_PI * pow(s,2));

    return dest;
}

void Rgb2Graphen::execute()
{
    const cv::Mat input = mIn;
    GrayImage dest(input.rows, input.cols);
    int mode = mMode;
    int target = mTarget;

    // load configuration if needed
    if (mSingleRGBString.hasChanged() || mSingleVarianceString.hasChanged() ||
            mDoubleRGBString.hasChanged() || mDoubleVarianceString.hasChanged())
    {
        loadRGBConfig();
    }

    // spliting input image into seperate channel specific matrixes
    std::vector<cv::Mat> channels(3);
    cv::split(input,channels);

    if(mode == BlueSubstractDefault)
    {
        cv::Mat diff;
        cv::absdiff(channels[1], channels[2], diff);
        // diff *= 0.5;
        channels[1]/=2;
        channels[2]/=2;
        cv::subtract(channels[0], channels[1], dest);
        cv::subtract(dest, channels[2], dest);
        cv::subtract(dest, diff, dest);
    }
    else if(mode == WeightedSummPaper)
    {
        cv::addWeighted(channels[2], 0.3, channels[1], 0.59, 0, dest);
        cv::addWeighted(dest, 1, channels[0], 0.11, 0, dest);
    }
    else if(mode == BlueSubstractTweeked) {
        //b-r+b-g
        cv::absdiff(channels[0],channels[2], dest);

        cv::Mat diff;
        cv::absdiff(channels[0],channels[1], diff);

        cv::add(diff, dest, dest);

        //dest.convertTo(dest, CV_64F);

        dest /= 4;

        channels[0] /= 2;
        cv::add(dest, channels[0], dest);
    }
    else if(mode == GaussSingle)
    {
        // factor to make differences visible
        const int factor = 128;
        const double v = mVariance.getValue();

        std::vector<cv::Mat> weighted(3);
        // use RGB values depending of set target
        switch(target)
        {
        case SingleLayerGraphen:
            weighted[0] = gauss(channels[2], mSingleVarianceVector.at(0)*v, mSingleRGBVector.at(0));
            weighted[1] = gauss(channels[1], mSingleVarianceVector.at(1)*v, mSingleRGBVector.at(1));
            weighted[2] = gauss(channels[0], mSingleVarianceVector.at(2)*v, mSingleRGBVector.at(2));
            break;
        case DoubleLayerGraphen:
            weighted[0] = gauss(channels[2], mDoubleVarianceVector.at(0)*v, mDoubleRGBVector.at(0));
            weighted[1] = gauss(channels[1], mDoubleVarianceVector.at(1)*v, mDoubleRGBVector.at(1));
            weighted[2] = gauss(channels[0], mDoubleVarianceVector.at(2)*v, mDoubleRGBVector.at(2));
            break;
        default:
            break;
        }

        // multiply the weight of every RGB-channel
        cv::Mat mul;
        cv::multiply(weighted[0], weighted[1], mul);
        cv::multiply(mul, weighted[2], mul);

        dest = mul * factor;
    }
    else if (mode == GaussCombined)
    {
        // factor to make differences visible
        const int singleFactor = 64;
        const int doubleFactor = 128;
        const double v = mVariance.getValue();

        std::vector<cv::Mat> singleWeighted(3);
        std::vector<cv::Mat> doubleWeighted(3);
        // use RGB values depending of set target
        singleWeighted[0] = gauss(channels[2], mSingleVarianceVector.at(0)*v, mSingleRGBVector.at(0));
        singleWeighted[1] = gauss(channels[1], mSingleVarianceVector.at(1)*v, mSingleRGBVector.at(1));
        singleWeighted[2] = gauss(channels[0], mSingleVarianceVector.at(2)*v, mSingleRGBVector.at(2));

        doubleWeighted[0] = gauss(channels[2], mDoubleVarianceVector.at(0)*v, mDoubleRGBVector.at(0));
        doubleWeighted[1] = gauss(channels[1], mDoubleVarianceVector.at(1)*v, mDoubleRGBVector.at(1));
        doubleWeighted[2] = gauss(channels[0], mDoubleVarianceVector.at(2)*v, mDoubleRGBVector.at(2));

        // multiply the weight of every RGB-channel
        // single layer component
        cv::Mat mul;
        cv::multiply(singleWeighted[0], singleWeighted[1], mul);
        cv::multiply(mul, singleWeighted[2], mul);

        dest = mul * singleFactor;

        // double layer component
        cv::multiply(doubleWeighted[0], doubleWeighted[1], mul);
        cv::multiply(mul, doubleWeighted[2], mul);

        mul *= doubleFactor;

        // add components
        cv::add(dest, mul, dest);
    }
    mOut.send(dest);
}
