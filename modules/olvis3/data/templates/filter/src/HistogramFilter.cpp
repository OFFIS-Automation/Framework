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

#include "HistogramFilter.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <QDebug>

using namespace cv;

REGISTER_FILTER(HistogramFilter);

HistogramFilter::HistogramFilter()
{
    
    setName("HistogramFilter");
    setDesc("Creates a histogram of the incoming image");
	setGroup("user");
	
    mIn.setName("input");
    mIn.setDesc("image input");
    addInputPort(mIn);
	
	mOut.setName("histogram");
    mOut.setDesc("the calculated histogram");
    addOutputPort(mOut);
}

void HistogramFilter::execute()
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
