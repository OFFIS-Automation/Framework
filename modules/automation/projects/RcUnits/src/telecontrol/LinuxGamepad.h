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

#ifndef RCUNITTOOLS_LINUXGAMEPAD_HQT
#define RCUNITTOOLS_LINUXGAMEPAD_HQT

#include "Gamepad.h"
#include "libjoystick/joystick.h"
#include <QMap>

class LinuxTelecontrolFactory;
class LinuxGamepad : public Gamepad
{
public:
    virtual ~LinuxGamepad();

protected:
    LinuxGamepad();
    virtual void run();
    virtual bool initialize();
    QString getName() { return mName; }
    int getResolution() { return 64; }
    GamepadType getGamepadType() { return Linux; }

    void update(QMap<int, double>& joysticks, QMap<int, bool>& buttons);
    float correctedValue(float v);

    Joystick mDevice;
    friend class LinuxTelecontrolFactory;
    QString mName;
};

#endif //RCUNITTOOLS_LINUXGAMEPAD_HQT
