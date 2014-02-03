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

#include "ROSOutput.h"
#include <QDebug>
#include <cv_bridge/cv_bridge.h>

REGISTER_FILTER(ROSOutput);

ROSOutput::ROSOutput()
{
    // Setup filter configuration
    setName("ROSOutput");
    setDesc("Sends image data to ROS on the specified topic");
    setGroup("ros");

    // Init ports
    mImageIn.setName("inImage");
    mImageIn.setDesc("image input port");
    addInputPort(mImageIn);

    mROSTopic.setName("rosTopic");
    mROSTopic.setDesc("ros topic to which the filter sends to");
    mROSTopic.setMode(OptionalPortMode);
    mROSTopic.setDefault("oaf/image");
    addInputPort(mROSTopic);

    mMode.setName("color mode");
    mMode.setDesc("Select the color mode.");
    mMode.addChoice(Mono8, "Default Mono, 8Bit");
    mMode.addChoice(Mono16, "Mono, 16Bit");
    mMode.addChoice(BGR8, "BGR, 8Bit");
    mMode.addChoice(BGRA8, "BGRA, 8Bit");
    mMode.addChoice(RGB8, "RGB, 8Bit");
    mMode.addChoice(RGBA8, "RGBA, 8Bit");
    mMode.setDefault(Mono8);
    mMode.setIcon(QImage(":/SimpleNodes/colorMode.png"));
    addInputPort(mMode);
}

void ROSOutput::execute()
{
    // Update subscriber with topic if string has changed
    if(mROSTopic.hasChanged()){
        // Create a nodehandle, NodeHandle is the main access point to communications with the ROS system.
        ros::NodeHandle n = getNodeHandle();
        // Subscribe to topic (at init time to default topic)
        mImagePublisher = n.advertise<sensor_msgs::Image>(mROSTopic.getValue().toStdString(), 10);
    }

    cv_bridge::CvImage cvImage;
    cvImage.image = mImageIn.getValue();
    cvImage.encoding = mode2str[mMode];

    // Publish image to topic
    mImagePublisher.publish(cvImage);
}

void ROSOutput::stop()
{
    // Call default
    UserFilter::stop();
}

void ROSOutput::deinitialize()
{
    // Call default
    UserFilter::deinitialize();
}

void ROSOutput::initialize()
{
    try {
        // Initialize ROS
        // ros::init() must be called before using any other part of the ROS system.
        int argc = 0;
        ros::init(argc, NULL, "oafImageSubscriber");

        if(ros::isInitialized()){
            // Create a nodehandle, NodeHandle is the main access point to communications with the ROS system.
            n = new ros::NodeHandle;
            // Subscribe to topic (at init time to default topic)
            mImagePublisher = n->advertise<sensor_msgs::Image>(mROSTopic.getValue().toStdString(), 10);
        } else {
            qWarning("ROS system is not initialized.");
        }

        // Call default
        UserFilter::initialize();
    } catch(const std::exception& err) {
        qCritical("ROS system could not be initialized: %s", err.what());
        return;
    }
}

void ROSOutput::start()
{
    // Call default
    UserFilter::start();
}
