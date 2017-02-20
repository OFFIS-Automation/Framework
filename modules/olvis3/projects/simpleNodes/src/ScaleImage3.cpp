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

#include "ScaleImage3.h"
#include <opencv2/imgproc.hpp>

REGISTER_FILTER(ScaleImage3);
ScaleImage3::ScaleImage3()
{
    setName("Scale3");
    setDesc(QObject::tr(""));
    setGroup("image/test");

    mOut.setName("imageOut");
    mOut.setDesc(QObject::tr("Image output"));
    addOutputPort(mOut);

    mScale.setName("scaleOut");
    mScale.setDesc(QObject::tr("Image output"));
    addOutputPort(mScale);

    mMask.setName("mask");
    mMask.setDesc(QObject::tr("Inverse output"));
    addOutputPort(mMask);

    mIn.setName("imageIn");
    mIn.setDesc(QObject::tr("Image input"));
    addInputPort(mIn);

    mMod.setName("mod");
    mMod.setDesc(QObject::tr("Modulo"));
    mMod.setDefault(2.0);
    addInputPort(mMod);
}

void ScaleImage3::execute()
{
    const int mod = mMod;
    const cv::Mat in = mIn;
    cv::Mat out = cv::Mat::zeros(in.rows, in.cols, in.type());
    cv::Mat inverse = cv::Mat::zeros(in.rows, in.cols, in.type());
    cv::Mat scale = cv::Mat(in.rows/mod, in.cols/mod, in.type());

    for(int i=0; i<in.rows; i++){
        for(int j=0; j<in.cols; j++){

            if(fmod(i, mod) == 0 && fmod(j, mod) == 0){
                if(in.depth() == CV_8U && in.channels() == 3){
                    out.at<cv::Vec3b>(i, j) = in.at<cv::Vec3b>(i, j);
                    scale.at<cv::Vec3b>(i/mod, j/mod) = in.at<cv::Vec3b>(i, j);
                    inverse.at<cv::Vec3b>(i, j) = cv::Vec3b(255,255,255);
                } else if(in.depth() == CV_8U && in.channels() == 1){
                    out.at<uchar>(i, j) = in.at<uchar>(i, j);
                    scale.at<uchar>(i/mod, j/mod) = in.at<uchar>(i, j);
                    inverse.at<uchar>(i, j) = 255;
                }
            }
        }
    }

    mOut.send(out);
    mScale.send(scale);
    mMask.send(inverse);
}
