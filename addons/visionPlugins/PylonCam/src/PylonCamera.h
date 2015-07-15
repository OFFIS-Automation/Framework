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

#ifndef USER_PYLONCAMERA_H
#define USER_PYLONCAMERA_H

#include <filter/PluginInterface.h>
#include <ports/ImagePort.h>
#include <ports/BooleanPort.h>
#include <pylon/PylonIncludes.h>
#include "params/ParamInterface.h"

/**
 * @brief The PylonCamera class implements a single camera controlled via the pylon interface
 * It supports all cameras currently supported by pylon @see README.txt
 */
class PylonCamera : public UserFilter
{
public:
    /**
     * creates an instance based on the camera info. Only the info is used, the camera is not opened
     * sets up name and description of the camera
     */
    PylonCamera(Pylon::CDeviceInfo info);
    /**
     * @brief initialize is called when a camera is added to the image processing chain.
     * Opens the camera and sets up a subset of camera parameters
     */
    virtual void initialize();

    /**
     * @brief start is called when the image processing system is started. It updates all parameters and calls startGrabbing on the camera
     */
    virtual void start();

    /**
     * @brief execute is called periodically. It updates parameters if neccessary and grabs an image from the camera nad sends it to the output port
     */
    virtual void execute();

    /**
     * @brief stop is called when the image processing is stopped. It calls stopGrabbing on the camera and updates all parameters
     */
    virtual void stop();

    /**
     * @brief deinitialize is called when the camera is removed from the image processing chain or the program is terminated. Closes the camera.
     */
    virtual void deinitialize();

protected:
    /**
     * @brief updateParams based on user changes and automatic changes by the camera,
     */
    void updateParams();
    /**
     * tries to create a parameter of the given template type for the given name.
     * @param name The name of the camera
     * @param vis The visibility of the port
     * @return true if the parameter could be created
     */
    template <class T> bool createParam(const QString& name, PortVisibility vis = AdvancedPortVisibility);

    /**
     * @brief mOut contains the main output port.
     */
    out::Image mOut;
    /**
     * @brief mParams contains a list of all valid parameters.
     */
    QList<ParameterInterface*> mParams;

    /**
     * @brief mCam contains the camera pointer. Only valid after initialize() and before deinitialize()
     */
    Pylon::CInstantCamera* mCam;

    /**
     * @brief mInfo contains the camera info
     */
    Pylon::CDeviceInfo mInfo;
};
#endif // USER_PYLONCAMERA_H
