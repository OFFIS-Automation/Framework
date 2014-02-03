// OFFIS Automation Framework
// Copyright (C) 2013 OFFIS e.V.
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

#ifndef ROSINPUT_H
#define ROSINPUT_H

#include <filter/PluginInterface.h>
#include <ports/StringPort.h>
#include <ports/ImagePort.h>

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/Image.h>

#include <QMutex>
#include <QWaitCondition>

#include "ROSSpinThread.h"

class ROSInput : public UserFilter
{
public:
    ROSInput();
    virtual void execute();
    virtual void initialize();
    virtual void start();
    virtual void stop();
    virtual void deinitialize();
protected:
    // Methods
    void imageCallback(const sensor_msgs::ImageConstPtr& imageMsg);
    ros::NodeHandle getNodeHandle(){ return *n; }

    // Attributes
    in::String mROSTopic;
    out::Image mImageOut;
    cv::Mat mImage;

    ros::Subscriber mImageSubscriber;
    ros::NodeHandle* n;
    ROSSpinThread mSpinThread;

    QMutex mMutex;
    QWaitCondition mCondition;
};

#endif // ROSINPUT_H
