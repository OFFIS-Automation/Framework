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

#include "BackgroundSubstraction.h"
#include <opencv2/imgproc.hpp>

REGISTER_FILTER(BackgroundSubstraction);
BackgroundSubstraction::BackgroundSubstraction()
{
    setName("BackgroundSubstraction");
    setDesc(QObject::tr("Automatic background subtraction"));
    setGroup("image/math");

    // Ports
    mMode.setName("mode");
    mMode.setDesc(QObject::tr("Select the operation mode"));
    mMode.addChoice(MOG2, "Default MOG2");
    mMode.addChoice(KNN, "KNN");
    mMode.setDefault(MOG2);
    mMode.setIcon(QImage(":/SimpleNodes/colorMode.png"));
    addInputPort(mMode);

    mIn.setName("imageIn");
    mIn.setDesc(QObject::tr("Image input"));
    addInputPort(mIn);

    mBackgroundOut.setName("background");
    mBackgroundOut.setDesc(QObject::tr("Extracted background from image input"));
    addOutputPort(mBackgroundOut);

    mForegroundOut.setName("foreground");
    mBackgroundOut.setDesc(QObject::tr("Extracted foreground from image input"));
    addOutputPort(mForegroundOut);

    // Create algorithm instances
    mMog2 = cv::createBackgroundSubtractorMOG2();
    mKNN = cv::createBackgroundSubtractorKNN();
}

void BackgroundSubstraction::execute()
{
    const cv::Mat src = mIn;
    cv::Mat background;

    if(mMode == KNN){
        mKNN->apply(src, mForegroundMask);
        mKNN->getBackgroundImage(background);
    } else {
        mMog2->apply(src, mForegroundMask);
        mMog2->getBackgroundImage(background);
    }

    mForegroundOut.send(mForegroundMask);
    mBackgroundOut.send(background);
}
