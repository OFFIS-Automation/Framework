// OFFIS Automation Framework
// Copyright (C) 2013-2018 OFFIS e.V.
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

#include "Overlay.h"
#include <opencv2/imgproc.hpp>

REGISTER_FILTER(Overlay);
Overlay::Overlay()
{
    setName("Overlay");
    setDesc(QObject::tr("Place an overlay on top of the input image."));
    setGroup("image/math");

    mIn.setName("imageIn");
    mIn.setDesc(QObject::tr("Image input"));
    addInputPort(mIn);

    mOut.setName("imageOut");
    mOut.setDesc(QObject::tr("Image output"));
    addOutputPort(mOut);

    mResizeOverlay.setName("resizeOverlay");
    mResizeOverlay.setDesc(QObject::tr("Set to true if overlay should be resized to the input image size."));
    mResizeOverlay.setDefault(false);
    addInputPort(mResizeOverlay);

    mOverlayIn.setName("overlayIn");
    addInputPort(mOverlayIn);
}

void Overlay::execute()
{
    // Convert to float values
    const cv::Mat input = mIn;
    const cv::Mat tmpOverlay = mOverlayIn;
    cv::Mat overlay;

    if(mResizeOverlay || tmpOverlay.size > input.size){
        // Resize overlay to input size
        cv::resize(tmpOverlay, overlay, input.size());
    } else {
        // Center overlay
        overlay = cv::Mat(input.size(), tmpOverlay.type());

        int xOffset = (input.cols - tmpOverlay.cols) / 2;
        int yOffset = (input.rows - tmpOverlay.rows) / 2;

        cv::copyMakeBorder(tmpOverlay, overlay, yOffset, yOffset, xOffset, xOffset, cv::BORDER_CONSTANT, cv::Scalar::all(0));
    }

    cv::Mat dest(input.size(), CV_8UC3);
    for(int y=0;y<dest.rows;y++){
        for(int x=0;x<dest.cols;x++)
        {
                int alpha = overlay.at<cv::Vec4b>(y,x)[3] / 255.0;
                //int alpha = 256 * (x+y)/(input4Ch.rows+input4Ch.cols);
                dest.at<cv::Vec3b>(y,x)[0] = (1-alpha) * input.at<cv::Vec3b>(y,x)[0] + alpha * overlay.at<cv::Vec4b>(y,x)[0];
                dest.at<cv::Vec3b>(y,x)[1] = (1-alpha) * input.at<cv::Vec3b>(y,x)[1] + alpha * overlay.at<cv::Vec4b>(y,x)[1];
                dest.at<cv::Vec3b>(y,x)[2] = (1-alpha) * input.at<cv::Vec3b>(y,x)[2] + alpha * overlay.at<cv::Vec4b>(y,x)[2];
        }
    }
    mOut.send(dest);
}
