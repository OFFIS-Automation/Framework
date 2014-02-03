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

#ifndef LOLECTOOLS_WINDOWSGAMEPAD_HQT
#define LOLECTOOLS_WINDOWSGAMEPAD_HQT

#include "Gamepad.h"

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <QMap>

class WindowsGamepadFactory;
class WindowsGamepad : public Gamepad
{
public:
    virtual int getResolution() const;
    virtual ~WindowsGamepad();
protected:
    WindowsGamepad(const QString& name, const QString& guid);
    virtual void run();
    virtual bool initialize();
    virtual QString getName() { return mName; }
    void createMapping();
    virtual void update(QMap<int, double>& joysticks, QMap<int, bool>& buttons);
    float correctedValue(float v);

    static BOOL CALLBACK enumObject(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);

    LPDIRECTINPUTDEVICE8 mDevice;
    DIJOYSTATE2 mState;
    friend class WindowsGamepadFactory;
    QString mName, mGuid;
    QMap<int, int> mButtonMapping;
    bool mSwitchZJoysticks;
    void assingButton(QMap<int, bool> &buttons, BYTE *data, int buttonId);
};

#endif //LOLECTOOLS_WINDOWSGAMEPAD_HQT
