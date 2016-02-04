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

#include "BlobDetection.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>
#include <QDebug>

REGISTER_FILTER(BlobDetection);


BlobDetection::BlobDetection()
{
    setName("BlobDetection");
    setDesc("Finds binary large objects in an image. The input image must be be thresholded");
    setGroup("image/object detection");
    mIn.setName("imageIn");
    addInputPort(mIn);
    mMinPixels.setName("minPixels");
    mMinPixels.setRange(0, INT_MAX);
    mMinPixels.setDefault(0);
    mMinPixels.setVisibility(AdvancedPortVisibility);
    mMaxPixels.setName("maxPixels");
    mMaxPixels.setRange(1, INT_MAX);
    mMaxPixels.setDefault(INT_MAX);
    mMaxPixels.setVisibility(AdvancedPortVisibility);
    mUseAxis.setName("useMainAxis");
    mUseAxis.setDesc("Enables the calculation of the objects main axis and PCe value");
    mUseAxis.setDefault(true);
    mUseAxis.setVisibility(AdvancedPortVisibility);
    mNumBlobs.setName("numBlobs");
    addOutputPort(mNumBlobs);
    addInputPort(mMinPixels);
    addInputPort(mMaxPixels);
    addInputPort(mUseAxis);
    addOutputListPort(mRects);
    addOutputListPort(mPolygons);
    addOutputListPort(mPoints);
    mNumPixels.setName("numPixels");
    addOutputListPort(mNumPixels);
    mPCe.setName("PCe");
    mPCe.setVisibility(AdvancedPortVisibility);
    addOutputListPort(mPCe);
    mOffsetIn.setName("offset");
    mOffsetIn.setDesc("Offset that will be added to all calculated positions");
    mOffsetIn.setDefault(QPointF(0.0, 0.0));
    mOffsetIn.setVisibility(AdvancedPortVisibility);
    addInputPort(mOffsetIn);
    mLines.setName("lines");
    mLines.setDesc("Lines in main direction, to detect the length");
    addOutputListPort(mLines);
    mPCe.setVisibility(ExpertPortVisibility);
}

void BlobDetection::execute()
{
    const cv::Mat source = mIn;
    cv::Mat image = source.clone();
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(image, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    int numBlobs = 0;
    for(uint i = 0; i< contours.size(); i++ )
    {
        if(hierarchy[i][3] >= 0) // this is an inner contour
            continue;
        std::vector<cv::Point> &contour = contours[i];
        cv::Moments mom = cv::moments(contour, true);
        if(mom.m00 > mMaxPixels || mom.m00 < mMinPixels)
            continue;
        numBlobs++;
        cv::Rect r = cv::boundingRect(contour);

        double alpha = 0.0;
        if(mUseAxis)
        {
            cv::Mat roi =source(r);
            cv::Mat contMat = cv::Mat::zeros(roi.size(), CV_8UC1);
            cv::drawContours(contMat, contours, i, cv::Scalar(255, 255, 255),CV_FILLED,8, cv::noArray(), INT_MAX, cv::Point(-r.x, -r.y));
            cv::Mat temp;
            cv::bitwise_and(contMat, roi, temp);
            cv::Mat pcaData(temp.cols*temp.rows, 2, CV_32FC1);
            int pcaId = 0;
            for(int y= 0; y < temp.rows; y++)
            {
                uchar* val = temp.ptr(y);
                for(int x=0;x<temp.cols; x++)
                {
                    if(val[x] == 0)
                        continue;
                    pcaData.at<float>(pcaId, 0) = y;
                    pcaData.at<float>(pcaId, 1) = x;
                    pcaId++;
                }
            }
            if(pcaId == 0)
                continue;
            pcaData = pcaData.rowRange(0, pcaId);
            cv::Mat coVar;
            cv::Mat mean;// = cv::Mat(1,2, CV_32FC1);
            cv::calcCovarMatrix(pcaData,coVar, mean,  CV_COVAR_NORMAL | CV_COVAR_ROWS, CV_32F);
            cv::Mat eigenValues, eigenVectors;
            cv::eigen(coVar,true,eigenValues, eigenVectors);
            alpha = atan2(eigenVectors.at<float>(0,0), eigenVectors.at<float>(0, 1));
            double eigen1 = eigenValues.at<float>(0,0);
            double eigen2 = eigenValues.at<float>(1,0);
            double pce = ((2* eigen1) / (eigen1 + eigen2)) -1;
            mPCe.send(pce);
        }
        else
            mPCe.send(0);
        QPointF offset = mOffsetIn;
        Pose2d point(offset.x() + mom.m10/mom.m00, offset.y() + mom.m01 / mom.m00, alpha);
        mPoints.send(point);
        QPolygonF poly;
        for(uint j=0;j<contour.size(); j++)
            poly << (port::Point::qpoint(contour[j]) + offset);


        mPolygons.send(poly);
        QRectF rect = port::Rect::qrect(r).translated(offset);
        mRects.send(rect);
        mNumPixels.send(mom.m00);
    }
    mNumBlobs.send(numBlobs);
}

