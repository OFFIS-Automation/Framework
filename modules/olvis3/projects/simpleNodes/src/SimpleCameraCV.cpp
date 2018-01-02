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

#include "SimpleCameraCV.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <QDebug>
#include <stdexcept>

REGISTER_FILTER(SimpleCameraCV);
SimpleCameraCV::SimpleCameraCV()
{
    setName("SimpleCameraCV");
    setDesc(QObject::tr("Outputs data from the default camera using cv::VideoCapture"));
    setGroup("input");

    mOut.setName("imageOut");
    mOut.setDesc(QObject::tr("Image output"));
    addOutputPort(mOut);

    mFps.setName("fps");
    mFps.setDesc(QObject::tr("Camera FPS. Must be connected to a minimum fps core to avoid sending the same image multiple times"));
    addOutputPort(mFps);
}

void SimpleCameraCV::initialize()
{
    if(!mCapture.open(0))
        throw std::runtime_error("Could not initialize webcam");
    mCapture.set(CV_CAP_PROP_CONVERT_RGB, 1);
}

void SimpleCameraCV::execute()
{
    mCapture.grab();
    cv::Mat image, image2;
    mCapture.retrieve(image);
    mOut.send(image.clone());
    mFps.send(mCapture.get(CV_CAP_PROP_FPS));
}

void SimpleCameraCV::deinitialize()
{
    if(mCapture.isOpened())
        mCapture.release();
}
