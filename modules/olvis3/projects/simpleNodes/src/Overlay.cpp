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

    if(mResizeOverlay){
        // Resize overlay to input size
        cv::resize(tmpOverlay, overlay, input.size());
    } else {
        // Center overlay
        overlay = cv::Mat(input.size(), tmpOverlay.type());

        int xOffset = (input.cols - tmpOverlay.cols) / 2;
        int yOffset = (input.rows - tmpOverlay.rows) / 2;

        cv::copyMakeBorder(tmpOverlay, overlay, yOffset, yOffset, xOffset, xOffset, cv::BORDER_CONSTANT, cv::Scalar::all(0));
    }

    // Change to the same number of channels
    cv::Mat input4Ch(input.size(), CV_MAKE_TYPE(input.type(), 4));
    cv::Mat overlay4Ch(overlay.size(), CV_MAKE_TYPE(overlay.type(), 4));

    int from_to[] = {0,0, 1,1, 2,2};
    cv::mixChannels(&input, 1, &input4Ch, 1, from_to, 3);
    cv::mixChannels(&overlay, 1, &overlay4Ch, 1, from_to, 3);

    cv::Mat dest;
    cv::addWeighted(input4Ch, 1.0, overlay4Ch, 1.0, 0.0, dest);

    mOut.send(dest);
}
