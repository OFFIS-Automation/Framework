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

#include "ScaleImage2.h"
#include <opencv2/imgproc.hpp>

REGISTER_FILTER(ScaleImage2);
ScaleImage2::ScaleImage2()
{
    setName("Spreading");
    setDesc(QObject::tr("Spreads an image by a factor"));
    setGroup("image/reshape");

    mOut.setName("imageOut");
    mOut.setDesc(QObject::tr("Image output"));
    addOutputPort(mOut);

    mIn.setName("imageIn");
    mIn.setDesc(QObject::tr("Image input"));
    addInputPort(mIn);

    mScale.setName("scale");
    mScale.setDesc(QObject::tr("The new image size in percent of the original image"));
    mScale.setRange(2, 8);
    mScale.setDefault(2.0);
    addInputPort(mScale);
}

void ScaleImage2::execute()
{
    const cv::Mat in = mIn;
    cv::Mat out = cv::Mat::zeros(in.rows * mScale, in.cols * mScale, in.type());

    for(int i=0; i<in.rows; i++){
        for(int j=0; j<in.cols; j++){
            if(in.depth() == CV_8U && in.channels() == 3){
                out.at<cv::Vec3b>(i*mScale, j*mScale) = in.at<cv::Vec3b>(i, j);
            } else if(in.depth() == CV_8U && in.channels() == 1){
                out.at<uchar>(i*mScale, j*mScale) = in.at<uchar>(i, j);
            }
        }
    }

    mOut.send(out);
}
