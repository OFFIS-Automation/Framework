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

#ifndef VIDEOINPUT_H
#define VIDEOINPUT_H
#include <filter/PluginInterface.h>
#include <ports/ImagePort.h>
#include <ports/FilePort.h>
#include <ports/RealPort.h>
#include <ports/BooleanPort.h>
#include <ports/IntegerPort.h>

#include <QThread>
#include <QElapsedTimer>
#include <Timer.h>

#include <opencv2/highgui/highgui.hpp>

class VideoInput : public UserFilter, QThread
{
public:
    VideoInput();
    virtual void execute();
    virtual void start();
    virtual void stop();
protected:
    in::ReadFile mIn;
    in::Boolean mRepeat;
    out::Image mOut;
    in::Real mStart, mEnd;
    in::Real mSpeed;

    out::Real mFpsOut;
    out::Real mPositionOut;
    out::Real mElapsedOut;

    QFileInfo mCurrentFile;
    double mFps;
    cv::VideoCapture mCapture;
    Timer mTimer;
};


#endif // VIDEOINPUT_H
