// OFFIS Automation Framework
// Copyright (C) 2013-2016 OFFIS e.V.
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

#ifndef WINDOWSGAMEPAD_H
#define WINDOWSGAMEPAD_H

#include "Gamepad.h"

#include <dinput.h>
#include <QMap>

class WindowsTelecontrolFactory;
class WindowsGamepad : public Gamepad
{
public:
    virtual ~WindowsGamepad();

protected:
    WindowsGamepad(const QString& name, const QString& guid);
    void run();
    bool initialize();
    QString getName() { return mName; }
    int getResolution() const;
    GamepadType getGamepadType() const { return Windows; }

    void createMapping();
    void assignButton(QMap<int, bool> &buttons, BYTE *data, int buttonId);
    void update(QMap<int, double>& joysticks, QMap<int, bool>& buttons);
    float correctedValue(float v);

    static BOOL CALLBACK enumObject(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);
    void changeConnexionMode(int buttonId);

    LPDIRECTINPUTDEVICE8 mDevice;
    DIJOYSTATE2 mState;
    friend class WindowsTelecontrolFactory;
    QString mName, mGuid;
    QMap<int, int> mButtonMapping;
    bool mSwitchZJoysticks;
    enum Type {
        XBoxGamepad,
        DefaultGamepad
    } mGamepadType;
};

#endif //WINDOWSGAMEPAD_H
