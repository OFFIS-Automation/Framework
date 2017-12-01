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

#include "Threshold.h"
#include <opencv2/imgproc.hpp>

REGISTER_FILTER(Threshold);
Threshold::Threshold()
{
    setName("Threshold");
    setDesc(tr("Thresholds an image<br>Input: 8C1"));
    setGroup("image/color");

    mOut.setName("imageOut");
    mOut.setDesc(tr("Image output"));
    addOutputPort(mOut);

    mIn.setName("imageIn");
    mIn.setDesc(tr("Image input"));
    addInputPort(mIn);

    mThreshold.setName("threshold");
    mThreshold.setDesc(tr("Threshold value"));
    mThreshold.setDefault(128);
    mThreshold.setRange(0, 255);
    addInputPort(mThreshold);

    mMode.setName("mode");
    mMode.setDesc(tr("Thresholding mode"));
    mMode.addChoice(cv::THRESH_BINARY, tr("Binary (value = value > threshold ? max_value : 0 )"));
    mMode.addChoice(cv::THRESH_BINARY_INV, tr("Binary inverted (value = value > threshold ? 0 : max_value)"));
    mMode.addChoice(cv::THRESH_TRUNC, tr("Truncate (value = value > threshold ? threshold : value)"));
    mMode.addChoice(cv::THRESH_TOZERO, tr("To zero (value = value > threshold ? value : 0)"));
    mMode.addChoice(cv::THRESH_TOZERO_INV, tr("To zero inverted (value = value > threshold ? 0 : value)"));
    mMode.addChoice(cv::THRESH_OTSU, tr("Otsu’s Algorithm (threshold automatically determined)"));
    mMode.addChoice(cv::THRESH_OTSU+1, tr("Otsu’s Algorithm inverted (threshold automatically determined)"));
    mMode.addChoice(cv::THRESH_TRIANGLE, tr("Triangle algorithm (threshold automatically determined)"));
    mMode.addChoice(cv::THRESH_TRIANGLE+1, tr("Triangle algorithm inverted (threshold automatically determined)"));
    mMode.setDefault(cv::THRESH_BINARY);

    mAdaptiveMode.setName("adaptiveMode");
    mAdaptiveMode.setDesc(tr("Only works with Binary or Binary inverted mode"));
    mAdaptiveMode.addChoice(-1, tr("None"));
    mAdaptiveMode.addChoice(cv::ADAPTIVE_THRESH_MEAN_C, tr("Threshold value is mean of the neighborhood minus C"));
    mAdaptiveMode.addChoice(cv::ADAPTIVE_THRESH_GAUSSIAN_C, tr("Threshold value is a weighted sum (cross-correlation with a Gaussian window) of the neighborhood minus C"));
    mAdaptiveMode.setDefault(-1);
    mAdaptiveMode.setVisibility(AdvancedPortVisibility);
    addInputPort(mAdaptiveMode);

    mAdaptiveBlockSize.setName("adaptiveBlockSize");
    mAdaptiveBlockSize.setDesc(tr("Size of the neighborhood"));
    mAdaptiveBlockSize.setOnlyOdd();
    mAdaptiveBlockSize.setDefault(3);
    mAdaptiveBlockSize.setVisibility(AdvancedPortVisibility);
    addInputPort(mAdaptiveBlockSize);

    mAdaptiveC.setName("adaptivC");
    mAdaptiveC.setDesc(tr("Constant subtracted from the mean or weighted mean"));
    mAdaptiveC.setOnlyOdd();
    mAdaptiveC.setDefault(5);
    mAdaptiveC.setVisibility(AdvancedPortVisibility);
    addInputPort(mAdaptiveC);

    addInputPort(mMode);
}

void Threshold::execute()
{
    int threshold = mThreshold;

    const cv::Mat src = mIn;
    cv::Mat srcConverted = src.clone();
    ((Image *)&srcConverted)->convertToGray(CV_8U);

    cv::Mat dest;
    if(mAdaptiveMode > 0){
        if(mMode == cv::THRESH_BINARY || mMode == cv::THRESH_BINARY_INV){
            cv::adaptiveThreshold(srcConverted, dest, 255, mAdaptiveMode, mMode, mAdaptiveBlockSize, mAdaptiveC);
        } else {
            throw std::runtime_error(qPrintable(tr("Mode not supported for adaptive threshold. Use Binary or Binary inverted.")));
        }
    } else {
        if(mMode == cv::THRESH_OTSU || mMode == cv::THRESH_TRIANGLE){
            cv::threshold(srcConverted, dest, 0, 255, cv::THRESH_BINARY + mMode);
        } else if(mMode == cv::THRESH_OTSU+1){
            cv::threshold(srcConverted, dest, 0, 255, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);
        }else if(mMode == cv::THRESH_TRIANGLE+1){
            cv::threshold(srcConverted, dest, 0, 255, cv::THRESH_BINARY_INV + cv::THRESH_TRIANGLE);
        } else {
            cv::threshold(srcConverted, dest, threshold, 255, mMode);
        }
    }
    mOut.send(dest);
}
