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

#include "MultipleTemplateMatching.h"
#include <opencv2/imgproc.hpp>

REGISTER_FILTER(MultipleTemplateMatching);

MultipleTemplateMatching::MultipleTemplateMatching()
{
    setName("MultipleTemplateMatching");
    setDesc(QObject::tr("Match a template on an image and output the all found positions, the matching scores and the result image"));
    setGroup("image/object detection");
    mImageIn.setName("image");
    addInputPort(mImageIn);

    mTemplateIn.setName("template");
    addInputPort(mTemplateIn);

    mMethodIn.setName("method");
    mMethodIn.addChoice(CV_TM_CCORR_NORMED,tr("normalized cross correlation"));
    mMethodIn.addChoice(CV_TM_CCOEFF_NORMED, tr("fast normalized cross correlation"));
    mMethodIn.addChoice(CV_TM_SQDIFF_NORMED, tr("normalized squared differences"));
    mMethodIn.setDefault(CV_TM_CCOEFF_NORMED);
    mMethodIn.setVisibility(AdvancedPortVisibility);
    addInputPort(mMethodIn);

    mMinScoreIn.setName("min score");
    mMinScoreIn.setRange(0.0,1.0);
    mMinScoreIn.setDefault(0.0);
    addInputPort(mMinScoreIn);

    mOffsetIn.setName("offset");
    mOffsetIn.setDesc(QObject::tr("Offset that will be added to all calculated positions"));
    mOffsetIn.setDefault(QPointF(0.0, 0.0));
    addInputPort(mOffsetIn);
    mOffsetIn.setVisibility(AdvancedPortVisibility);

    mScoreOuts.setName("scores");
    mScoreOuts.setDesc(QObject::tr("The matching score of the found position"));
    addOutputListPort(mScoreOuts);

    mPositionOuts.setName("positions");
    addOutputListPort(mPositionOuts);

    mNeedNewImage.setName("fresh image only");
    mNeedNewImage.setDesc(QObject::tr("Only execute if the main input has a new image"));
    mNeedNewImage.setDefault(true);
    mNeedNewImage.setVisibility(AdvancedPortVisibility);
    addInputPort(mNeedNewImage);

    mSendResultImage.setName("send result image");
    mSendResultImage.setDesc(QObject::tr("Set this to true if the result image of the template matching should be send on the port <result>"));
    mSendResultImage.setDefault(false);
    mSendResultImage.setVisibility(ExpertPortVisibility);
    addInputPort(mSendResultImage);

    mResultOut.setName("result");
    mResultOut.setDesc(QObject::tr("The result image from the template matching operation. Only sent if the port <send result image> is set to true"));
    mResultOut.setVisibility(ExpertPortVisibility);
    addOutputPort(mResultOut);
}

void MultipleTemplateMatching::start(){}

void MultipleTemplateMatching::execute()
{
    // Only execute for new image data
    if(mNeedNewImage.getValue() && !mImageIn.isUpdated())
        return;

    // Init data
    const cv::Mat imageIn = mImageIn;
    cv::Mat image = imageIn;
    const cv::Mat templateIn = mTemplateIn;
    cv::Mat tpl = templateIn;

    // Convert to right data format
    if(image.channels() == 3 && tpl.channels() != 3)
        cv::cvtColor((cv::Mat)mTemplateIn, tpl, CV_GRAY2RGB);
    else if(image.channels() == 1 && tpl.channels() != 1)
        cv::cvtColor((cv::Mat)mTemplateIn, tpl, CV_RGB2GRAY);

    cv::Mat result;
    cv::Point2d offset(tpl.cols/2.0, tpl.rows/2.0);
    cv::Point2d imageOffset = mOffsetIn;
    cv::matchTemplate(image, tpl, result, mMethodIn);
    cv::threshold(result, result, (double)mMinScoreIn, 1., CV_THRESH_TOZERO);

    if((bool)mSendResultImage){
        cv::Mat resultSend;
        result.convertTo(resultSend, CV_8UC1, 255);
        mResultOut.send(resultSend);
    }

    while (true){
        double minVal, maxVal, threshold = (double)mMinScoreIn;
        cv::Point minLoc, maxLoc;
        cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

        double score = 0.0;
        cv::Point2d pos;

        if(mMethodIn == CV_TM_SQDIFF_NORMED){
            score = 1.0 - minVal;
            pos = cv::Point2d((CvPoint)minLoc);
        } else {
            score = maxVal;
            pos = cv::Point2d((CvPoint)maxLoc);
        }

        if(score > threshold){
            mScoreOuts.send(score);
            mPositionOuts.send(imageOffset + offset + pos);
            cv::floodFill(result, pos, cv::Scalar(0), 0, cv::Scalar(.1), cv::Scalar(1.));
        } else {
            break;
        }
    }
}
