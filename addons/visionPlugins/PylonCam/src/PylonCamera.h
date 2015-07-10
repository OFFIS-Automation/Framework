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
class PylonCamera : public UserFilter
{
public:
    PylonCamera(Pylon::CDeviceInfo info);
    virtual void initialize();
    virtual void start();
    virtual void execute();
    virtual void stop();
    virtual void deinitialize();

protected:
    template <class T> bool createParam(const QString& name, PortVisibility vis = AdvancedPortVisibility);
    out::Image mOut;
    QList<ParameterInterface*> mParams;
    Pylon::CInstantCamera* mCam;
    Pylon::CDeviceInfo mInfo;
};
#endif // USER_PYLONCAMERA_H
