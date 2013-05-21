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

#include "LinuxGamepad.h"

#include <QMap>
#include <QDebug>
#include <QElapsedTimer>
#include <stdexcept>

#include <telecontrol/TcConfig.h>

bool LinuxGamepad::initialize()
{
    mDevice = Joystick(QString("/dev/input/js0").toLocal8Bit().data());

    if(mDevice.connected == 1){
        setObjectName("Gamepad: " + QString(mDevice.getName()));
        return true;
    } else {
        qWarning() << tr("Could not initialize gamepad on port /dev/input/js0");
        return false;
    }
}

void LinuxGamepad::update(QMap<int, double> &joysticks, QMap<int, bool> &buttons)
{
    // Axis (and their local buttons)
    joysticks[Tc::XAxisLeft] = correctedValue(float(mDevice.getAxis(0) / 255.0));
    joysticks[Tc::YAxisLeft] = correctedValue(-float(mDevice.getAxis(1) / 255.0));
    buttons[Tc::LeftAxisPush] = mDevice.getButton(10) != 0;
    joysticks[Tc::XAxisRight] = correctedValue(float(mDevice.getAxis(2) / 255.0));
    joysticks[Tc::YAxisRight] = correctedValue(-float(mDevice.getAxis(3) / 255.0));
    buttons[Tc::RightAxisPush] = mDevice.getButton(10) != 0;

    // Buttons
    buttons[Tc::Button1] = mDevice.getButton(0) != 0;
    buttons[Tc::Button2] = mDevice.getButton(1) != 0;
    buttons[Tc::Button3] = mDevice.getButton(2) != 0;
    buttons[Tc::Button4] = mDevice.getButton(3) != 0;
    buttons[Tc::Button5] = mDevice.getButton(4) != 0;
    buttons[Tc::Button6] = mDevice.getButton(5) != 0;
    buttons[Tc::Button7] = mDevice.getButton(6) != 0;
    buttons[Tc::Button8] = mDevice.getButton(7) != 0;
    buttons[Tc::Button9] = mDevice.getButton(8) != 0;
    buttons[Tc::Button10] = mDevice.getButton(9) != 0;

    // D-Pad
    buttons[Tc::ButtonLeft] = mDevice.getAxis(4) < 0;
    buttons[Tc::ButtonRight] = mDevice.getAxis(4) > 0;
    buttons[Tc::ButtonUp] = mDevice.getAxis(5) > 0;
    buttons[Tc::ButtonDown] = mDevice.getAxis(5) < 0;
}

float LinuxGamepad::correctedValue(float v)
{
    v /= (float)getResolution();
    if(v >-0.1 && v < 0.1)
        return 0.0;
    else
        return v;
}

LinuxGamepad::LinuxGamepad()
{
    mDevice = 0;
}

int LinuxGamepad::getResolution() const
{
    return 64;
}

LinuxGamepad::~LinuxGamepad()
{
    stop();
    wait();
    if(mDevice.connected == 1)
        mDevice.stop();
}

void LinuxGamepad::run()
{
    mStop = false;
    QMap<int,double> joysticks;
    QMap<int,bool> buttons, lastButtons;
    while(!mStop)
    {
        update(joysticks, buttons);
        QMapIterator<int,bool> iter(buttons);
        while(iter.hasNext()){
            iter.next();
            int id = iter.key();
            bool value = iter.value();
            if(lastButtons.contains(id)){
                if(lastButtons[id] != value) // value has changed
                emit buttonToggled(id, value);
            } else if(value) // first run, emit if pressed
                emit buttonToggled(id, value);
        }
        lastButtons = buttons;
        emit dataUpdate(joysticks);
    }
}
