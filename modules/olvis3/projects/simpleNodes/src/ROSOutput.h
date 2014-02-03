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

#ifndef ROSOUTPUT_H
#define ROSOUTPUT_H

#include <filter/PluginInterface.h>
#include <ports/StringPort.h>
#include <ports/ImagePort.h>
#include <ports/IntegerPort.h>

#include <ros/ros.h>
#include <sensor_msgs/Image.h>

#include <QMutex>
#include <QWaitCondition>

class ROSOutput : public UserFilter
{
public:
    ROSOutput();
    virtual void execute();
    virtual void initialize();
    virtual void start();
    virtual void stop();
    virtual void deinitialize();
protected:
    enum Mode
    {
        Mono8 = 0,
        Mono16,
        BGR8,
        BGRA8,
        RGB8,
        RGBA8
    };

    const char* mode2str[6] = {"mono8", "mono16", "bgr8", "bgra8", "rgb8", "rgba8"};

    // Methods
    ros::NodeHandle getNodeHandle(){ return *n; }

    // Attributes
    in::String mROSTopic;
    in::Image mImageIn;
    in::Integer mMode;

    ros::Publisher mImagePublisher;
    ros::NodeHandle* n;
};

#endif // ROSINPUT_H
