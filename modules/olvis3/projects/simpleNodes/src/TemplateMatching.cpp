// OFFIS Automation Framework
// Copyright (C) 2013-2014 OFFIS e.V.
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

#include "TemplateMatching.h"
#include <opencv2/imgproc/imgproc.hpp>

REGISTER_FILTER(TemplateMatching);

TemplateMatching::TemplateMatching()
{
    setName("TemplateMatching");
    setDesc("Match a template on an image and output the best position, the matching score and the result image.");
    setGroup("image/object detection");
    mImageIn.setName("image");
    addInputPort(mImageIn);

    mTemplateIn.setName("template");
    addInputPort(mTemplateIn);
    mMethodIn.setName("method");
    mMethodIn.addChoice(CV_TM_CCORR_NORMED,"normalized cross correlation");
    mMethodIn.addChoice(CV_TM_CCOEFF_NORMED,"fast normalized cross correlation");
    mMethodIn.addChoice(CV_TM_SQDIFF_NORMED,"normalized squared differences");
    mMethodIn.setDefault(CV_TM_CCOEFF_NORMED);
    mMethodIn.setVisibility(AdvancedPortVisibility);
    addInputPort(mMethodIn);

    mMinScoreIn.setName("min score");
    mMinScoreIn.setRange(0.0,1.0);
    mMinScoreIn.setDefault(0.0);
    addInputPort(mMinScoreIn);

    mFastSearchRange.setName("fast search");
    mFastSearchRange.setDesc("set the fast search pixels. The search area is the template rectange at the last found position plus this value on each border. A value of zero means no fast search.");
    mFastSearchRange.setDefault(0);
    mFastSearchRange.setRange(0,9999);
    mFastSearchRange.setVisibility(AdvancedPortVisibility);
    addInputPort(mFastSearchRange);

    mSendInvalidPos.setName("send invalid pos");
    mSendInvalidPos.setDesc("If set to false, no position will be send if the score is to low. If true, an invalid position (-1, -1) will be send");
    mSendInvalidPos.setDefault(false);
    mSendInvalidPos.setVisibility(AdvancedPortVisibility);
    addInputPort(mSendInvalidPos);

    mSendResultImage.setName("send result image");
    mSendResultImage.setDesc("Set this to true if the result image of the template matching should be send on the port <result>");
    mSendResultImage.setDefault(false);
    mSendResultImage.setVisibility(ExpertPortVisibility);
    addInputPort(mSendResultImage);

    mOffsetIn.setName("offset");
    mOffsetIn.setDesc("Offset that will be added to all calculated positions");
    mOffsetIn.setDefault(QPointF(0.0, 0.0));
    addInputPort(mOffsetIn);

    mResultOut.setName("result");
    mResultOut.setDesc("The result image from the template matching operation. Only sent if the port <send result image> is set to true");
    mResultOut.setVisibility(ExpertPortVisibility);
    addOutputPort(mResultOut);


    mScoreOut.setName("score");
    mScoreOut.setDesc("The matching score of the found position");
    addOutputPort(mScoreOut);
    mPositionOut.setName("position");
    addOutputPort(mPositionOut);

    mFastSearchOut.setName("search area");
    mFastSearchOut.setDesc("The area in the input image where the template was searched");
    mFastSearchOut.setVisibility(ExpertPortVisibility);
    addOutputPort(mFastSearchOut);

    mResultOffset.setName("result offset");
    mResultOffset.setDesc("The offset of the result image from the original image. Only sent if the port <send result image> is set to true");
    mResultOffset.setVisibility(ExpertPortVisibility);
    addOutputPort(mResultOffset);

    mNeedNewImage.setName("fresh image only");
    mNeedNewImage.setDesc("Only execute if the main input has a new image");
    mNeedNewImage.setDefault(true);
    mResultOffset.setVisibility(AdvancedPortVisibility);
    addInputPort(mNeedNewImage);

}

void TemplateMatching::start()
{
   mLastPos = cv::Point(-1,-1);
}

void TemplateMatching::execute()
{
    if(mNeedNewImage.getValue() && !mImageIn.isUpdated())
        return;
    const cv::Mat imageIn = mImageIn;
    cv::Mat image = imageIn;
    cv::Mat result;
    const cv::Mat templateIn = mTemplateIn;
    cv::Mat tpl = templateIn;
    if(image.channels() == 3 && tpl.channels() != 3)
        cv::cvtColor((cv::Mat)mTemplateIn, tpl, CV_GRAY2RGB);
    else if(image.channels() == 1 && tpl.channels() != 1)
        cv::cvtColor((cv::Mat)mTemplateIn, tpl, CV_RGB2GRAY);
    int searchPixels = mFastSearchRange;
    QRectF roi(0,0,image.cols, image.rows);
    cv::Point2d offset(tpl.cols/2.0, tpl.rows/2.0);
    if(mLastPos.x >= 0 && searchPixels > 0)
    {
        roi = QRect(mLastPos.x - tpl.cols/2 - searchPixels,
                    mLastPos.y - tpl.rows/2 - searchPixels,
                    tpl.cols+ 2*searchPixels,
                    tpl.rows + 2*searchPixels);

        roi = roi.intersected(QRectF(0,0, image.cols, image.rows));
        image = image(port::Rect::rect(roi));
        offset.x += roi.x();
        offset.y += roi.y();
    }

    cv::matchTemplate(image, tpl, result, mMethodIn);
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;

    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
    double score = 0.0;
    cv::Point2d pos;

    if(mMethodIn == CV_TM_SQDIFF_NORMED)
    {
        score = 1.0 - minVal;
        pos = cv::Point2d((CvPoint)minLoc);
    }
    else
    {
        score = maxVal;
        pos = cv::Point2d((CvPoint)maxLoc);
    }
    mScoreOut.send(score);
    mLastPos = cv::Point(-1,-1);


    cv::Point2d imageOffset = mOffsetIn.getValue();

    if(score >= (double)mMinScoreIn)
    {
        mPositionOut.send(offset + imageOffset + pos);
        if(mFastSearchRange)
            mLastPos = offset + pos;
    }
    else if((bool)mSendInvalidPos)
        mPositionOut.send(cv::Point2d(-1,-1));
    if((bool)mSendResultImage)
    {
        result.convertTo(result, CV_8UC1, 255);
        mResultOut.send(result);
        mResultOffset.send(offset+imageOffset);
    }
    mFastSearchOut.send(roi);

}
