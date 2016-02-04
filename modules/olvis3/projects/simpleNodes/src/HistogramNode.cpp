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

#include "HistogramNode.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <QDebug>

using namespace cv;

REGISTER_FILTER(HistogramNode);

HistogramNode::HistogramNode()
{
    setGroup("image/analysis");
    setName("Histogram");
    setDesc("Creates a histogram of the incoming image");

    mIn.setName("imageIn");
    mIn.setDesc("Image input");
    addInputPort(mIn);

    mOut.setName("historgram");
    mOut.setDesc("Historgram output port");
    addOutputPort(mOut);
}

void HistogramNode::execute()
{
    const cv::Mat img = mIn;
    std::vector<cv::Mat> images;
    cv::split(img, images);
    std::vector<cv::Mat> hists(images.size());
    for(unsigned i=0; i<images.size(); i++)
    {
        int sizes[] =  { 256 };
        int channels[] = { 0 };
        float range[] = {0, 256 };
        const float* ranges[] = { range };
        calcHist(&images[i], 1, channels, Mat(), hists[i], 1, sizes, ranges);
    }
    Mat result;
    if(hists.size() == 1)
        result = hists[0];
    else if(hists.size()>1)
        cv::merge(hists, result);
    mOut.send(result);
}
