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

#include "ROSInput.h"
#include <QDebug>

REGISTER_FILTER(ROSInput);

ROSInput::ROSInput()
{
    // Setup filter configuration
    setName("ROSInput");
    setDesc("Receives image data from ROS on the specified topic");
    setGroup("ros");

    // Init ports
    mImageOut.setName("outImage");
    mImageOut.setDesc("image output port");
    addOutputPort(mImageOut);

    mROSTopic.setName("rosTopic");
    mROSTopic.setDesc("ros topic to which the filter subscribes to");
    mROSTopic.setMode(OptionalPortMode);
    mROSTopic.setDefault("oaf/image");
    addInputPort(mROSTopic);
}

void ROSInput::execute()
{
    // Update subscriber with topic if string has changed
    if(mROSTopic.hasChanged()){
        // Create a nodehandle, NodeHandle is the main access point to communications with the ROS system.
        ros::NodeHandle n = getNodeHandle();
        // Subscribe to topic (at init time to default topic)
        mImageSubscriber = n.subscribe(mROSTopic.getValue().toStdString(), 10, &ROSInput::callbackImageReceived, this);
    }
}

void ROSInput::stop()
{
    mSpinThread.terminate();
    // Call default
    UserFilter::stop();
}

void ROSInput::deinitialize()
{
    // Call default
    UserFilter::deinitialize();
}

void ROSInput::initialize()
{
    // Initialize ROS
    // ros::init() must be called before using any other part of the ROS system.
    char* argv; int argc = 0;
    ros::init(argc, &argv, "oafImageListener");

    if(ros::isInitialized()){
        // Create a nodehandle, NodeHandle is the main access point to communications with the ROS system.
        n = new ros::NodeHandle;
        // Subscribe to topic (at init time to default topic)
        mImageSubscriber = n->subscribe(mROSTopic.getValue().toStdString(), 10, &ROSInput::callbackImageReceived, this);
    } else {
        qWarning() << tr("ROS system could not be initialized.");
    }

    // Call default
    UserFilter::initialize();
}

void ROSInput::start()
{
    mSpinThread.start();
    // Call default
    UserFilter::start();
}

// Callbacks
void ROSInput::callbackImageReceived(const sensor_msgs::ImageConstPtr &imageMsg)
{

}
