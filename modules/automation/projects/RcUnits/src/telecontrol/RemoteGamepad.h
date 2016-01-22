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

#ifndef RCUNITTOOLS_REMOTEGAMEPAD_HQT
#define RCUNITTOOLS_REMOTEGAMEPAD_HQT

#include "Gamepad.h"

class TelecontrolFactory;
class RemoteGamepad : public Gamepad
{
public:
    virtual ~RemoteGamepad();

public slots:
    void onRemoteGamepadDataUpdated(const QMap<int, double>& data, const QString& gamepadName);
    void onRemoteGamepadButtonToggled(int buttonId, const bool& pressed, const QString& gamepadName);

protected:
    RemoteGamepad(const QString& name);
    QString getName() { return mName; }
    GamepadType getGamepadType() { return GamepadType::Remote; }
    friend class TelecontrolFactory;
    QString mName;
};

#endif //RCUNITTOOLS_REMOTEGAMEPAD_HQT
