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

#include "VideoInput.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QDebug>
#include <stdexcept>

REGISTER_FILTER(VideoInput);

VideoInput::VideoInput()
{
    setName("VideoInput");
    setDesc("Reads a video data stream and outputs the single frames (with variable frame rate). The fps-rate of this processor in always >= the requested fps");
    setGroup("input");

    // Inputs
    mIn.setName("filename");
    mIn.setDesc("File to read video data stream from");
    mIn.setFilter("*.avi *.wmv *.m4v *.mp4");
    addInputPort(mIn);

    mRepeat.setName("repeat");
    mRepeat.setDefault(true);
    mRepeat.setDesc("Repeat from beginning if the video data stream has finished");
    mRepeat.setIcon(QImage(":/SimpleNodes/repeat.png"));
    mRepeat.setVisibility(AdvancedPortVisibility);
    addInputPort(mRepeat);

    mSpeed.setName("speed");
    mSpeed.setDesc("Playback speed");
    mSpeed.setDefault(1.0);
    mSpeed.addChoice(0.01, "0.01x");
    mSpeed.addChoice(0.1, "0.1x");
    mSpeed.addChoice(0.25, "0.25x");
    mSpeed.addChoice(0.5, "0.5x");
    mSpeed.addChoice(1.0, "Normal");
    mSpeed.addChoice(2.0, "2x");
    mSpeed.addChoice(4.0, "4x");
    mSpeed.addChoice(10.0, "10x");
    mSpeed.addChoice(1000.0, "fastest");

    addInputPort(mSpeed);
    /*
    mFpsMode.setName("fpsMode");
    mFpsMode.setDesc("Active wait is cpu-time consuming, but on most systems more precise than sleep wait");
    mFpsMode.addChoice(SleepMode, "Sleep wait");
    mFpsMode.addChoice(ActiveWaitMode, "Active wait");
    mFpsMode.setDefault(SleepMode);
    addInputPort(mFpsMode);
*/
    mStart.setName("startPosition");
    mStart.setDesc("Start offset (range: 0.0 to 1.0)");
    mStart.setRange(0.0, 1.0);
    mStart.setDefault(0.0);
    mStart.setVisibility(AdvancedPortVisibility);
    addInputPort(mStart);
    mEnd.setName("endPosition");
    mEnd.setDesc("End offset (range: 0.0 to 1.0)");
    mEnd.setRange(0.0, 1.0);
    mEnd.setDefault(1.0);
    mEnd.setVisibility(AdvancedPortVisibility);
    addInputPort(mStart);
    addInputPort(mEnd);


    // Outputs
    mFpsOut.setName("outFps");
    mFpsOut.setDesc("Original fps of input video data stream");
    addOutputPort(mFpsOut);

    mPositionOut.setName("outPosition");
    mPositionOut.setDesc("Position of the current video file (range: 0.0 to 1.0)");
    mPositionOut.setVisibility(AdvancedPortVisibility);
    addOutputPort(mPositionOut);

    mElapsedOut.setName("outElapsed");
    mElapsedOut.setDesc("Elapsed time of the current video file in seconds");
    mElapsedOut.setVisibility(AdvancedPortVisibility);
    addOutputPort(mElapsedOut);

    mOut.setName("outImage");
    mOut.setDesc("Image output port");
    addOutputPort(mOut);
}

void VideoInput::start()
{
    mCurrentFile = QFileInfo();
    mTimer.start();
}

void VideoInput::stop()
{
    if(mCapture.isOpened())
        mCapture.release();
}

void VideoInput::execute()
{
    QFileInfo path = mIn;
    if(mCurrentFile != path)
    {
        if(mCapture.isOpened())
            mCapture.release();
        mCapture.open(path.canonicalFilePath().toStdString());
        if(!mCapture.isOpened())
            throw std::runtime_error("Could not read video stream");

        mCurrentFile = path;
        int framePos = qRound(mCapture.get(CV_CAP_PROP_FRAME_COUNT) * (double)mStart);
        mCapture.set(CV_CAP_PROP_POS_FRAMES, framePos);

    }
    double elapsedFrames = mCapture.get(CV_CAP_PROP_POS_FRAMES) / mCapture.get(CV_CAP_PROP_FRAME_COUNT);
    if(!mCapture.grab() || elapsedFrames > (double) mEnd)
    {
        // Check if *.avi has finished
        if((bool)mRepeat){
            // Reset frame pos to start pos
            int framePos = qRound(mCapture.get(CV_CAP_PROP_FRAME_COUNT) * (double)mStart);
            mCapture.set(CV_CAP_PROP_POS_FRAMES, framePos);
            if(!mCapture.grab())
                return;
        } else {
            return;
        }
    }
    double realFps = mCapture.get(CV_CAP_PROP_FPS);
    double factor = mSpeed;
    if(factor == 0) factor = 0.001;
    double duration = 1000.0 / (realFps * factor);
    double elapsed = mTimer.elapsed();
    if(elapsed < duration && duration > 0){
        msleep(duration-elapsed);
    }

    // Restart timer
    mTimer.restart();

    // Send image
    cv::Mat img;
    if(!mCapture.retrieve(img))
        return;

    mFpsOut.send(realFps);
    mPositionOut.send(mCapture.get(CV_CAP_PROP_POS_FRAMES) / mCapture.get(CV_CAP_PROP_FRAME_COUNT));
    mElapsedOut.send(mCapture.get(CV_CAP_PROP_POS_MSEC)/1000.0);

    mOut.send(img.clone());
}
