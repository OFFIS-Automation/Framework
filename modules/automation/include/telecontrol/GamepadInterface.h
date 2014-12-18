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

#ifndef GAMEPADINTERFACE_H
#define GAMEPADINTERFACE_H

#include <QObject>
#include <QStringList>
#include <QMap>
#include "TcConfig.h"

struct GamepadStatus {
    long    lX;                     /* x-axis position              */
    long    lY;                     /* y-axis position              */
    long    lZ;                     /* z-axis position              */
    long    lRx;                    /* x-axis rotation              */
    long    lRy;                    /* y-axis rotation              */
    long    lRz;                    /* z-axis rotation              */
    long    rglSlider[2];           /* extra axes positions         */
    unsigned long rgdwPOV[4];             /* POV directions               */
    unsigned char rgbButtons[128];        /* 128 buttons                  */
    long    lVX;                    /* x-axis velocity              */
    long    lVY;                    /* y-axis velocity              */
    long    lVZ;                    /* z-axis velocity              */
    long    lVRx;                   /* x-axis angular velocity      */
    long    lVRy;                   /* y-axis angular velocity      */
    long    lVRz;                   /* z-axis angular velocity      */
    long    rglVSlider[2];          /* extra axes velocities        */
    long    lAX;                    /* x-axis acceleration          */
    long    lAY;                    /* y-axis acceleration          */
    long    lAZ;                    /* z-axis acceleration          */
    long    lARx;                   /* x-axis angular acceleration  */
    long    lARy;                   /* y-axis angular acceleration  */
    long    lARz;                   /* z-axis angular acceleration  */
    long    rglASlider[2];          /* extra axes accelerations     */
    long    lFX;                    /* x-axis force                 */
    long    lFY;                    /* y-axis force                 */
    long    lFZ;                    /* z-axis force                 */
    long    lFRx;                   /* x-axis torque                */
    long    lFRy;                   /* y-axis torque                */
    long    lFRz;                   /* z-axis torque                */
    long    rglFSlider[2];          /* extra axes forces            */
};

class GamepadInterface
{
public:
    virtual QStringList getNames() = 0;
    virtual int getResolution() const = 0;
    virtual void update(const GamepadStatus& status, QMap<int, double>& joysticks, QMap<int, bool>& buttons) const = 0;
};
#define GamepadInterface_iid "com.amir.hilec.GamepadInterface/1.0"
Q_DECLARE_INTERFACE(GamepadInterface, GamepadInterface_iid)

#endif // GAMEPADINTERFACE_H
