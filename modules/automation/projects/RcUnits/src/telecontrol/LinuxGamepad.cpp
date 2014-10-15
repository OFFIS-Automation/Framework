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
    joysticks[Tc::LeftJoystickX] = correctedValue(float(mDevice.getAxis(0) / 255.0));
    joysticks[Tc::LeftJoystickY] = correctedValue(-float(mDevice.getAxis(1) / 255.0));
    joysticks[Tc::RightJoystickX] = correctedValue(float(mDevice.getAxis(2) / 255.0));
    joysticks[Tc::RightJoystickY] = correctedValue(-float(mDevice.getAxis(3) / 255.0));

    // Buttons
    buttons[Tc::WestButton] = mDevice.getButton(0) != 0;
    buttons[Tc::SouthButton] = mDevice.getButton(1) != 0;
    buttons[Tc::EastButton] = mDevice.getButton(2) != 0;
    buttons[Tc::NorthButton] = mDevice.getButton(3) != 0;
    buttons[Tc::LeftShoulderUpperButton] = mDevice.getButton(4) != 0;
    buttons[Tc::RightShoulderUpperButton] = mDevice.getButton(5) != 0;
    buttons[Tc::LeftShoulderLowerButton] = mDevice.getButton(6) != 0;
    buttons[Tc::RightShoulderLowerButton] = mDevice.getButton(7) != 0;
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
            int buttonId = iter.key();
            bool value = iter.value();
            if(lastButtons.contains(buttonId)){
                if(lastButtons[buttonId] != value) // value has changed
                emit buttonToggled(buttonId, value);
            } else if(value) // first run, emit if pressed
                emit buttonToggled(buttonId, value);
        }
        lastButtons = buttons;
        emit dataUpdate(joysticks);
    }
}
