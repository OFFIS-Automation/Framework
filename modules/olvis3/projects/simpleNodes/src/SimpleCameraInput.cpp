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

#include "SimpleCameraInput.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QDebug>
#include <stdexcept>

REGISTER_FILTER(SimpleCameraInput);

SimpleCameraInput::SimpleCameraInput()
{
    setName("Webcam");
    setDesc("Reads a the default webcam with the default setup");
    setGroup("input");
    mOut.setName("outImage");
    mOut.setDesc("image output port");
    addOutputPort(mOut);
    mFps.setName("fps");
    mFps.setDesc("Camera fps. Must be connected to a minimum fps core to avoid sending the same image multiple times");
    addOutputPort(mFps);
}

void SimpleCameraInput::initialize()
{
    if(!mCapture.open(0))
        throw std::runtime_error("could not initialize webcam");
    mCapture.set(CV_CAP_PROP_CONVERT_RGB, 1);
}

void SimpleCameraInput::execute()
{
    mCapture.grab();
    cv::Mat image, image2;
    mCapture.retrieve(image);
    mOut.send(image.clone());
    mFps.send(mCapture.get(CV_CAP_PROP_FPS));
}
