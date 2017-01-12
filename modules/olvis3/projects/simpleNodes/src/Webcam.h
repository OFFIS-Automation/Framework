// OFFIS Automation Framework
// Copyright (C) 2013-2017 OFFIS e.V.
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

#ifndef WEBCAM_H
#define WEBCAM_H

#include "CameraFrameGrabber.h"

#include <filter/PluginInterface.h>
#include <ports/ImagePort.h>
#include <ports/StringPort.h>

#include <QCamera>
#include <QMutex>

class Webcam : public UserFilter
{
    Q_OBJECT
public:
    Webcam();
    virtual void execute();
    virtual void initialize();
    virtual void deinitialize();

public slots:
    void handleFrame(QImage frame);

protected:
    in::String mCameraName;
    out::Image mOut;

    QImage mCurrentImage;
    QCamera *mCamera;
    CameraFrameGrabber *mCameraFrameGrabber;
    QMutex mImageMutex;
};


#endif // WEBCAM_H
