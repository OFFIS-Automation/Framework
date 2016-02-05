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

#include "Rgb2Gray.h"
#include <opencv2/imgproc/imgproc.hpp>

REGISTER_FILTER(Rgb2Gray);
Rgb2Gray::Rgb2Gray()
{
    setName("RgbToGray");
    setDesc(QObject::tr("Converts an RGB image into a grayscale image. Can extract a single channel"));
    setGroup("image/color");
    mMode.setName("channel");
    mMode.setDesc(QObject::tr("Select the color channel"));
    mMode.addChoice(GrayscaleMode, "Default RGB->Gray");
    mMode.addChoice(BlueMode, "Blue");
    mMode.addChoice(GreenMode, "Green");
    mMode.addChoice(RedMode, "Red");
    mMode.addChoice(BlueSubstractMode, "B- R/2 - G/2 - diff(R,G)/2");
    mMode.addChoice(GreenSubstractMode, "G - R/2 - B/2 - diff(R,B)/2");
    mMode.addChoice(RedSubstractMode, "R - B/2 - G/2 - diff(B,G)/2");
    mMode.addChoice(MinMode, "min(R,G,B)");
    mMode.addChoice(MaxMode, "max(R,G,B)");
    mMode.setDefault(GrayscaleMode);
    mMode.setIcon(QImage(":/SimpleNodes/colorMode.png"));
    addInputPort(mMode);

    mOut.setName("imageOut");
    mOut.setDesc(QObject::tr("Image output"));
    addOutputPort(mOut);

    mIn.setName("imageIn");
    mIn.setDesc(QObject::tr("Image input"));
    addInputPort(mIn);
}

void Rgb2Gray::execute()
{
    const cv::Mat input = mIn;
    GrayImage dest(input.rows, input.cols);
    int mode = mMode;
    if(mode == MinMode || mode == MaxMode)
    {
        std::vector<cv::Mat> channels(3);
        cv::split(input,channels);
        const cv::Mat& b(channels[0]);
        const cv::Mat& g(channels[1]);
        const cv::Mat& r(channels[2]);


        if(mode == MinMode)
            dest = cv::min(b, cv::min(g, r));
        else
            dest = cv::max(b, cv::max(g, r));
    }
    else if(mode == BlueSubstractMode || mode == GreenSubstractMode || mode == RedSubstractMode)
    {
        int channel = mode-BlueSubstractMode;
        std::vector<cv::Mat> channels(3);
        cv::split(input,channels);
        int other1 = (channel+1)%3;
        int other2 = (channel+2)%3;
        cv::Mat diff;
        cv::absdiff(channels[other1], channels[other2], diff);
        //diff *= 0.5;
        channels[other1]/=2;
        channels[other2]/=2;
        cv::subtract(channels[channel], channels[other1], dest);
        cv::subtract(dest, channels[other2], dest);
        cv::subtract(dest, diff, dest);
    }
    else if(mode == BlueMode || mode == GreenMode || mode == RedMode)
    {
        int channel = mode-BlueMode;
        int mix[] = {channel, 0};
        cv::mixChannels(&input, 1, &dest, 1, mix, 1);
    }
    else
    {
        cv::cvtColor(input, dest, CV_BGR2GRAY);
    }
    mOut.send(dest);
}
